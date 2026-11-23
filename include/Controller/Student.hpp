#pragma once
#include <crow.h>
#include "../Models/Student.hpp"
#include "../database/Clubhub.hpp"
#include <sstream>
#include <ctime>
#include "../service/hash.hpp"
#include "../Models/Token.hpp"
#include "../service/KeyGenerate.hpp"
#include <sqlite_orm/sqlite_orm.h>
#include <string>
using namespace sqlite_orm;
inline std::string currentDate(){
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&now));
    return buffer;
}
inline crow::json::wvalue ToJson(const Student& student ){
    crow::json::wvalue result;
    result["id"]=student.id;
    result["student_id"]=student.studentId;
    result["name"]=student.name;
    result["email"]=student.email;
    result["password"]=student.password;
    result["createdAt"]=student.createdAt;
    return result;
}
inline void RegisterStudentRoute(crow::SimpleApp & app,Storage & db){
    CROW_ROUTE(app,"/api/Student/Register")
    .methods(crow::HTTPMethod::Post)
    ([&db](const crow::request& req)  {
        auto body= crow::json::load(req.body);
        if(!body){
            return crow::response(400,"invalid json");
        }
        if(!body.has("email") || !body.has("studentId") || !body.has("name")){
            return crow::response(400,"Plz input all the option");
        }
        try{
            init();
            Student student;
            student.studentId=body["studentId"].s();
            student.email=body["email"].s();
            student.name=body["name"].s();
            student.password=hash(body["password"].s());
            student.studentPhone=body["studentPhone"].s();
            student.createdAt=currentDate();
            auto id =db.insert(student);
            student.id=static_cast<int>(id);
            Token token;
            token.OwerId=student.id;
            token.createdAt=currentDate();
            std::string generated=generateToken();
            token.tokenHash=hash(generated);
            db.insert(token);
            crow::json::wvalue result;
            result["message"]="fetch done";
            result["data"]=ToJson(student);
            result["token"]=generated;
            return crow::response(201,result);

        }catch(const std::exception& e){
            std::ostringstream error;
            error<<"Server not working "<<e.what();
            return crow::response(500,error.str());
        }
    });
    CROW_ROUTE(app,"/api/Students")([&db](){
        try{
            auto student=db.get_all<Student>();
            crow::json::wvalue result;
            int index=0;
            for(const Student& s: student){
                result[index++]=ToJson(s);
            }
            return crow::response(201,result);
        }catch(const std::exception& e){
            return crow::response(500,"sever is not working ");
        }
    });
    // ?Fix how to make it compare with int and string or id and token to make it more secure 
    //! And fixx a ton of thing
    CROW_ROUTE(app,"/api/Student/<int>/<string>")([&db](int id,std::string token ){
        Token tok;
        std::string hashed=hash(token);
        auto user=db.get<Token>(where(c(&Token::tokenHash)==hashed));
        auto userwithID=db.get<Token>(id);
        if(user.OwerId==id){
            crow::json::wvalue json;
            json["id"]=user.OwerId;
            json["tokenhash"]=user.tokenHash;
            json["data"]=user.OwerId;   
            return crow::response(201,json);
        }
        crow::json::wvalue result;
        result["id"]=userwithID.id;
        result["owerId"]=userwithID.OwerId;
        result["what the hell"]=userwithID.tokenHash;
        return crow::response(401,result);
    });
    CROW_ROUTE(app,"/api/Students/<int>").methods(crow::HTTPMethod::Delete)([&db](int id){
        try{
            db.remove<Student>(id);
            return crow::response(200,"Succesfully deleted!");
        }catch(const std::exception& e){
            return crow::response(500,"Sever not working");
        }
    });
    CROW_ROUTE(app,"/api/Student/<int>").methods(crow::HTTPMethod::Put)([&db](const crow::request& req,int id){
        try{    
            auto student=db.get<Student>(id);
            auto body=crow::json::load(req.body);
            if(!body){
            return crow::response(400,"invalid json");
        }
        if(!body.has("email") || !body.has("studentId") || !body.has("name")){
            return crow::response(400,"Plz input all the option");
        }
        student.studentId=body["studentId"].s();
        student.email=body["email"].s();
        student.name=body["name"].s();
        db.update(student);
        return crow::response(201,"Updated successfully");
        }catch(const std::exception& e){
            return crow::response(500,"Server not working right now");
        }
    });
};