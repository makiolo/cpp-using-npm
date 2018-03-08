#include <iostream>
#include <string>
#include <cppunix/parallel_scheduler.h>
#include <cppunix/channel.h>
#include <cppunix/shell.h>
#include <cppunix/rest.h>
#include <spdlog/fmt/fmt.h>
#include <json.hpp>


std::string url_encode(const std::string &value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
	{
		std::string::value_type c = (*i);

		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char) c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}


int main()
{
	using namespace fmt::literals;
	using json = nlohmann::json;

	std::cout << "begin" << std::endl;

	// while(true)
	// {

	std::string hostname = "localhost";
	int port = 8086;
	std::string database = "domotica";
	cu::parallel_scheduler sch;
	cu::channel<json> read(sch);
	read.pipeline( cu::get() );
	sch.spawn([&](auto& yield) {
		std::cout << "begin production" << std::endl;
		std::string q, parameters, url, username;
		database = "domotica";
		username = "root";
		q = "SELECT \"price\" FROM \"binance\" WHERE (\"symbol\" =~ /^WTCBTC$/) AND time >= now() - 24h GROUP BY \"symbol\";SELECT \"hold_buy_price\" FROM \"portfolio_analisys\" WHERE (\"type\" = 'buy' AND \"symbol\" =~ /^WTCBTC$/) AND time >= now() - 24h GROUP BY \"symbol\"";
		q = url_encode(q);
		parameters = "q={}&db={}&u={}&p"_format(q, database, username);
		parameters = "q=SELECT+%22price%22+FROM+%22binance%22+WHERE+(%22symbol%22+%3D~+%2F%5EWTCBTC%24%2F)+AND+time+%3E%3D+now()+-+24h+GROUP+BY+%22symbol%22%3BSELECT+%22hold_buy_price%22+FROM+%22portfolio_analisys%22+WHERE+(%22type%22+%3D+'buy'+AND+%22symbol%22+%3D~+%2F%5EWTCBTC%24%2F)+AND+time+%3E%3D+now()+-+24h+GROUP+BY+%22symbol%22&db=domotica&u=root&p=";

		url = "http://{}:{}/query?{}"_format(hostname, port, parameters);

		std::cout << url << std::endl;
		read(yield, json{ {"url", url} });
		read.close(yield);
		std::cout << "end production" << std::endl;
	});
	sch.spawn([&](auto& yield) {
		std::cout << "begin consume" << std::endl;
		auto to_string = [](auto elem) -> std::string {
			if(!elem.is_null())
			{
				// escape space, comma or equal
				std::string data = elem.template get<std::string>();
				data = cu::replace_all(data, " ", "\\ ");
				data = cu::replace_all(data, ",", "\\,");
				data = cu::replace_all(data, "=", "\\=");
				return data;
			}
			else
				return "";
		};
		auto to_float = [](auto elem) -> float {
			if(!elem.is_null())
				return std::stof(elem.template get<std::string>());
			else
				return 0.0f;
		};
		auto to_integer = [](auto elem) -> int {
			if(!elem.is_null())
				return std::stoi(elem.template get<std::string>());
			else
				return 0;
		};
		std::string measurement = "coinmarketcap2";
		for(auto& jsn : cu::range(yield, read))
		{
			std::cout << "---------------------" << std::endl;
			std::cout << jsn << std::endl;
			std::cout << "---------------------" << std::endl;
			auto node = jsn["last_updated"];
			if(!node.is_null())
			{
				auto id = to_string(jsn["id"]);
				auto name = to_string(jsn["name"]);
				auto symbol = to_string(jsn["symbol"]);
				auto rank = to_integer(jsn["rank"]);
				auto price_eur = to_float(jsn["price_eur"]);
				auto last_updated = to_integer(jsn["last_updated"]);
				bool ok = cu::curl_post(
					"http://{}:{}/write?db={}"_format(hostname, port, database), 
					"{},id={},name={},symbol={} rank={},price_eur={} {}"_format(
						measurement, 
						id, name, symbol, 
						rank, price_eur,
						last_updated));
				if(ok)
				{
					std::cout << "ok" << std::endl;
				}
				else
				{
					std::cout << "error en post" << std::endl;
				}
			}
		}
		std::cout << "end consume" << std::endl;
	});
	sch.run_until_complete();

	// }

	std::cout << "end" << std::endl;
	return 0;
}

