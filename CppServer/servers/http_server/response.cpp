#include "response.hpp"


namespace code_templates {
    std::string bad_req_template =
            "<h1 align=\"center\">Bad Request</h1>";

    std::string not_found_template =
            "<h1 align=\"center\">Not Found<h1>";
}

namespace responses {
    const char *code_to_html(response::status_code code) {
        switch (code) {
            case response::status_code::bad_request:
                return code_templates::bad_req_template.c_str();
                break;
            case response::status_code::not_found:
                return code_templates::not_found_template.c_str();
                break;
            default:
                return "Unknown code";
                break;
        }
    }
}

std::string response::handle_math(request &req) {
    auto op = req.header().req_data.at(0).value;
    math math;

    if (op == "arithmetic") {
        auto expression = req.header().req_data.at(1).value;

        std::string buf = std::to_string(math.shuntingYard(expression).evaluatePostfix());
        return buf;
    } else if (op == "diff") {
        float at;
        auto func = req.header().req_data.at(1).value;
        try {
            at = atoi(req.header().req_data.at(2).value.c_str());
        }
        catch (const std::exception &ex) {
            std::cerr << ex.what() << "\n";
        }

        std::string buf;
        return buf;
    } else {
        std::string buf = "Unknown operation" + op;
        return buf;
    }
}

std::string response::buildResponse(request &req) {
    std::string buf;

    if (req.isGood()) {
        buf = load_template_file(req);
    } else {
        buf = responses::code_to_html(status_code::bad_request);
    }
    return buf;
}


std::string response::load_template_file(request &req) {
    auto path = req.header().path;
    std::vector<char> buf;
    std::string full_path;

    if (!req.header().req_data.empty() && req.header().method == "POST") {
        if (req.header().path == "/secret") {
            //Arithmetic dedicated page
            return handle_math(req);
        } else {
            boost::thread
            post_req_thread(boost::bind(&response::handle_post_request, this, req));
            return "<script>alert('Data is written to database');</script>";
        }
    }
    //construct a full path
    if (path == "/") {
        full_path = template_dir_ + "/" + "greet.html";
    } else if (path == "/users") {
        return retrieve_user_data(req);
    } else {
        if (path.substr(path.length() - 4, path.length()) != ".css" &&
            path.substr(path.length() - 3, path.length()) != ".js") {

            full_path = template_dir_ + "/" + path + ".html";
        } else {
            full_path = static_dir_ + '/' + path;
        }
    }
    std::ifstream in(full_path, std::ios::in | std::ios::binary);
    if (in.is_open()) {
        in.seekg(std::ios::beg, std::ios::end);
        size_t size = in.tellg();
        in.seekg(0);
        buf.resize(size);
        in.read(buf.data(), size);
        std::string buffer = buf.data();
        make_clean(buffer);
        return buffer;
    } else {
        std::string not_found_res = code_templates::not_found_template;
        return not_found_res.c_str();
    }
}

std::string response::retrieve_user_data(request &req) {
    std::string host = "127.0.0.1";
    auto port = 5600;
    endp_obj db_endp(ADDR_FROM_STR(host), 5600);

    //request_stream << json;
    asio_ctx ctx;
    sock_ptr_t sock_ = boost::make_shared<sock_t>(ctx);
    try {
        sock_->connect(db_endp);
    }
    catch (boost::system::system_error er) {
        Logger::log(SEVERITY::ERR, er.what());
        return "<h1>Unable to connect to a database server</h1>";
    }

    auto hostname = "127.0.0.1";

    std::string request_buffer;
    std::ostringstream buf;

    request_buffer += "GET /api/users HTTP/1.1\r\n";
    request_buffer += "Host: " + host + "\r\n";
    request_buffer += "Accept: */*\r\n";
    request_buffer += "Connection: close\r\n\r\n";

    err_code err;
    sync_client cl{sock_, request_buffer};
    cl(err);
    auto hdpair = utils::split_response(cl.respstr);
    auto pretty = utils::prettify_json(hdpair.second);
    if (err.cause == "") {
        return "<p>" + pretty + "</p>";
    } else {
        Logger::log(SEVERITY::ERR, err.cause);
        return "";
    }
}

void response::make_clean(std::string &text) {
    auto last = text.find_last_of(">;");
    text.erase(last + 1, text.length() - 1);
}

std::string response::handle_post_request(request &req) {
    std::string host = "127.0.0.1";
    auto port = 5600;
    endp_obj db_endp(ADDR_FROM_STR(host), port);

    //request_stream << json;
    asio_ctx ctx;
    sock_ptr_t sock_ = boost::make_shared<sock_t>(ctx);
    sock_->connect(db_endp);

    auto hostname = "127.0.0.1";

    std::string request_buffer;
    std::ostringstream buf;

    ptree root;

    auto email = req.header().req_data.at(0).value;
    auto first_name = req.header().req_data.at(1).value;
    auto last_name = req.header().req_data.at(2).value;
    auto password = req.header().req_data.at(3).value;
    auto username = req.header().req_data.at(4).value;

    std::string url_encoded_data = "username=" + username + "&first_name=" + first_name +
                                   "&last_name=" + last_name + "&email=" + email + "&password=" + password;

    request_buffer += "POST /api/create_user HTTP/1.1\r\n";
    request_buffer += "Content-Type: application/x-www-form-urlencoded\r\n";
    request_buffer += "Host: " + host + "\r\n";
    request_buffer += "Accept: */*\r\n";
    request_buffer += "Content-Length: " + std::to_string(url_encoded_data.length()) + "\r\n";
    request_buffer += "Connection: close\r\n\r\n";
    request_buffer += url_encoded_data;

    err_code err;
    sync_client{ sock_, request_buffer }(err);
    if (!err.cause.empty()) {
        Logger::log(SEVERITY::ERR, err.cause);
        return "";
    }

    sock_->close();
    return "Je'm balade sur l'avenue";
}


void response::connect_handler(const errc &err_conn) {

}


void response::write_handler(const errc &err, size_t bytes) {
    if (!err) {
        async_read(sock, resp, boost::bind(&response::read_handler, this,
                                           placeholders::error, placeholders::bytes_transferred));
    } else {
        std::cerr << err.value();
    }
}

void response::read_handler(const errc &err, size_t bytes) {
    if (!err) {
        std::string resp_str((std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>());

        std::cout << resp_str;
    } else {
        std::cout << err.value();
    }
}
