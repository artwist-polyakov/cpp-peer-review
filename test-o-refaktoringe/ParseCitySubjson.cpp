#include <string>
#include <vector>

using namespace std;
using Language = string;

class Country {
public:
    friend class CountryBuilder;

    Country() = default;

    string GetName() const {
        return name;
    }

    string GetIsoCode() const {
        return iso_code;
    }

    string GetPhoneCode() const {
        return phone_code;
    }

    string GetTimeZone() const {
        return time_zone;
    }

    vector<Language> GetLanguages() const {
        return languages;
    }


private :
    string name;
    string iso_code;
    string phone_code;
    string time_zone;
    vector<Language> languages;
    void CheckHealth() {
        if (name.empty() || iso_code.empty() || phone_code.empty() || time_zone.empty() || languages.empty()) {
            throw invalid_argument("Country is not healthy"s);
        }
    }

    Country &SetName(string name) {
        this->name = move(name);
        return *this;
    }

    Country &SetIsoCode(string iso_code) {
        this->iso_code = move(iso_code);
        return *this;
    }

    Country &SetPhoneCode(string phone_code) {
        this->phone_code = move(phone_code);
        return *this;
    }

    Country &SetTimeZone(string time_zone) {
        this->time_zone = move(time_zone);
        return *this;
    }

    Country &SetLanguages(vector<Language> languages) {
        this->languages = move(languages);
        return *this;
    }
};

class CountryBuilder {
public:
    CountryBuilder() = default;

    CountryBuilder &SetName(string name) {
        country.SetName(move(name));
        return *this;
    }

    CountryBuilder &SetIsoCode(string iso_code) {
        country.SetIsoCode(move(iso_code));
        return *this;
    }

    CountryBuilder &SetPhoneCode(string phone_code) {
        country.SetPhoneCode(move(phone_code));
        return *this;
    }

    CountryBuilder &SetTimeZone(string time_zone) {
        country.SetTimeZone(move(time_zone));
        return *this;
    }

    CountryBuilder &SetLanguages(vector<Language> languages) {
        country.SetLanguages(move(languages));
        return *this;
    }

    Country Build() {
        country.CheckHealth();
        return move(country);
    }
private:
    Country country = Country();

};

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country.GetPhoneCode() + city_obj["phone_code"s].AsString(), country.GetName(), country.GetIsoCode(),
                          country.GetTimeZone(), country.GetLanguages()});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();]

        countries.push_back(CountryBuilder()
                                    .SetName(country_obj["name"s].AsString())
                                    .SetIsoCode(country_obj["iso_code"s].AsString())
                                    .SetPhoneCode(country_obj["phone_code"s].AsString())
                                    .SetTimeZone(country_obj["time_zone"s].AsString())
                                    .SetLanguages(country_obj["languages"s].AsList()).Build());
        Country& country = countries.back();
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
}