struct LoadPersonsParams {
    string db_name;
    int db_connection_timeout = 0;
    bool db_allow_exceptions = false;
    DBLogLevel db_log_level = DBLogLevel::Error;
    int min_age;
    int max_age;
    string name_filter;

    LoadPersonsParams() {
        if (!db_name.empty() || min_age == 0 || max_age == 0) {
            throw invalid_argument("Invalid params"s);
        }
    }

    LoadPersonsParams& SetDBName(string name) {
        db_name = move(name);
        return *this;
    }

    LoadPersonsParams& SetDBConnectionTimeout(int timeout) {
        db_connection_timeout = timeout;
        return *this;
    }

    LoadPersonsParams& SetDBAllowExceptions(bool allow_exceptions) {
        db_allow_exceptions = allow_exceptions;
        return *this;
    }

    LoadPersonsParams& SetDBLogLevel(DBLogLevel log_level) {
        db_log_level = log_level;
        return *this;
    }

    LoadPersonsParams& SetMinAge(int age) {
        min_age = age;
        return *this;
    }

    LoadPersonsParams& SetMaxAge(int age) {
        max_age = age;
        return *this;
    }

    LoadPersonsParams& SetNameFilter(string name) {
        name_filter = move(name);
        return *this;
    }

};


vector<Person> LoadPersons(LoadPersonsParams params) {
    DBConnector connector(params.db_allow_exceptions, params.db_log_level);
    DBHandler db;
    if (params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params.db_name, params.db_connection_timeout);
    } else {
        db = connector.Connect(params.db_name, params.db_connection_timeout);
    }
    if (!params.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << params.min_age << " and "s << params.max_age << " "s
              << "and Name like '%"s << db.Quote(params.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}