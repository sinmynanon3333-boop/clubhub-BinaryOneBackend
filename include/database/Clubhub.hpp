#include <sqlite_orm/sqlite_orm.h>
#include "../Models/Student.hpp"
#include "../Models/TopUser.hpp"
#include "../Models/Token.hpp"
inline auto createStorage(const std::string& path ="data/Clubhub.db"){
    using namespace sqlite_orm;
    return make_storage(
        path,
        make_table(
            "Student",
            make_column("id",&Student::id,primary_key().autoincrement()),
            make_column("student_id",&Student::studentId,unique()),
            make_column("name",&Student::name),
            make_column("studentPhone",&Student::studentPhone,unique()),
            make_column("password",&Student::password),
            make_column("createdAt",&Student::createdAt)
        ),
        make_table(
            "TopLevelUser",
            make_column("id",&TopUser::id,primary_key().autoincrement()),
            make_column("name",&TopUser::name),
            make_column("PhoneNumber",&TopUser::PhoneNumber),
            make_column("password",&TopUser::password),
            make_column("email",&TopUser::email),
            make_column("role",&TopUser::role)
        ),
        make_table(
        "tokens",
        make_column("id", &Token::id, primary_key().autoincrement()),
        make_column("token_hash", &Token::tokenHash, unique()),
        make_column("owner_type", &Token::role),
        make_column("owner_id", &Token::OwerId),
        make_column("created_at", &Token::createdAt),
        make_column("expires_at", &Token::experiedAt),
        make_column("revoked", &Token::revoked)
)
    );
}
using Storage=decltype(createStorage());
    inline Storage initDatabase(){
        auto storage = createStorage();
        storage.sync_schema();
        return storage;
    }