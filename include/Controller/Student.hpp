#pragma once
#include <crow.h>
#include "../Models/Student.hpp"
#include "../database/Clubhub.hpp"
#include <sstream>
#include <ctime>
inline std::string currentDate(){
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&now));
    return buffer;
}
inline crow::json::wvalue ToJson(const Student& student){
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
    CROW_ROUTE(app,"/api/Student")
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
            Student student;
            student.studentId=body["studentId"].s();
            student.email=body["email"].s();
            student.name=body["name"].s();
            student.password=body["password"].s();
            student.studentPhone=body["studentPhone"].s();
            student.createdAt=currentDate();
            auto id =db.insert(student);
            student.id=static_cast<int>(id);
            crow::json::wvalue result;
            result=ToJson(student);
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
    CROW_ROUTE(app,"/api/Student/<int>")([&db](int id ){
        auto student=db.get<Student>(id);
        crow::json::wvalue result;
        result=ToJson(student);
        return crow::response(201,result);
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
}