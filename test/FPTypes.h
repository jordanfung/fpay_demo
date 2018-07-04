#ifndef __BC_TYPES_H__
#define __BC_TYPES_H__

#include <string>
#include <vector>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

typedef enum {
    COMMAND_NONE = 0,
    COMMAND_QUERY = 1,
    COMMAND_CONNECT = 2,
    COMMAND_CONFIRMATION = 3,
    COMMAND_PAY = 4,
    COMMAND_BROADCAST = 5,
} Command;

class Query 
{
public:
    enum { uri = COMMAND_QUERY};  
    std::string _address;

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(cereal::make_nvp("addr", _address));
    }
    static bool ToString(const Query & tx, std::string& json)
    {
        std::ostringstream ss;
        try {
            cereal::JSONOutputArchive archive(ss);
            archive(tx);
        }
        catch (cereal::Exception& e) {
           // LOG(WARNING) << e.what();
        }

        json = ss.str();
        return !json.empty();
    }


};


class Confirm
{
public:
    std::string _address;
    uint64_t _timestamp;

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(cereal::make_nvp("addr", _address), cereal::make_nvp("ts", _timestamp));
    }
};

class Transaction
{
public:
    std::string _id;
    std::string _from;
    std::string _to;
    int32_t _value;
    int32_t _timestamp;
    //std::vector<Confirm> _confirmList;

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(cereal::make_nvp("id", _id), 
           cereal::make_nvp("from", _from),
           cereal::make_nvp("to", _to), 
           cereal::make_nvp("value", _value), 
           cereal::make_nvp("ts", _timestamp));/*,
           cereal::make_nvp("confirms", _confirmList));*/
    }

    static bool ToString(const Transaction& tx, std::string& json)
    {
        std::ostringstream ss;
        try {
            cereal::JSONOutputArchive archive(ss);
            archive(tx);
        }
        catch (cereal::Exception& e) {
           // LOG(WARNING) << e.what();
        }

        json = ss.str();
        return !json.empty();
    }

    static bool FromString(const std::string& json, Transaction& tx)
    {
        std::istringstream is(json);
        try {
            cereal::JSONInputArchive archive(is);
            archive(tx);
            return true;
        }
        catch (cereal::Exception& e) {
           // LOG(WARNING) << e.what();
        }

        return false;
    }
};

class Block
{
public:
    std::string _Id;
    std::string _prev;
    std::string _next;
    uint64_t _timestamp;
    std::vector<Transaction> _txList;

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(cereal::make_nvp("id", _Id),
           cereal::make_nvp("prev", _prev),
           cereal::make_nvp("next", _next),
           cereal::make_nvp("ts", _timestamp),
           cereal::make_nvp("txs", _txList));
    }

    static bool ToString(const Block& block, std::string json)
    {
        std::ostringstream ss;
        try {
            cereal::JSONOutputArchive archive(ss);
            archive(block);
        }
        catch (cereal::Exception& e) {
           // LOG(WARNING) << e.what();
        }

        json = ss.str();
        return !json.empty();
    }

    static bool FromString(const std::string& json, Block& block)
    {
        std::istringstream is(json);
        try {
            cereal::JSONInputArchive archive(is);
            archive(block);
            return true;
        }
        catch (cereal::Exception& e) {
           // LOG(WARNING) << e.what();
        }

        return false;
    }
};

#endif
