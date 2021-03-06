#ifndef __CERBERUS_COMMAND_HPP__
#define __CERBERUS_COMMAND_HPP__

#include <set>
#include <vector>

#include "utils/pointer.h"
#include "buffer.hpp"

namespace cerb {

    class Proxy;
    class Client;
    class Server;
    class CommandGroup;

    class Command {
    public:
        Buffer buffer;
        util::sref<CommandGroup> const group;
        bool const need_send;

        virtual ~Command() {}

        virtual Server* select_server(Proxy* proxy) = 0;
        virtual void copy_response(Buffer rsp, bool error);

        Command(Buffer b, util::sref<CommandGroup> g, bool s)
            : buffer(std::move(b))
            , group(g)
            , need_send(s)
        {}

        Command(util::sref<CommandGroup> g, bool s)
            : group(g)
            , need_send(s)
        {}

        Command(Command const&) = delete;
    };

    class CommandGroup {
    public:
        util::sref<Client> const client;

        CommandGroup(util::sref<Client> cli)
            : client(cli)
        {}

        CommandGroup(CommandGroup const&) = delete;
        virtual ~CommandGroup() {}

        virtual bool long_connection() const
        {
            return false;
        }

        virtual void deliver_client(Proxy*) {}
        virtual bool wait_remote() const = 0;
        virtual void select_remote(std::set<Server*>& servers, Proxy* proxy) = 0;
        virtual void append_buffer_to(std::vector<util::sref<Buffer>>& b) = 0;
        virtual int total_buffer_size() const = 0;
        virtual void command_responsed() = 0;
    };

    void split_client_command(Buffer& buffer, util::sref<Client> cli);

}

#endif /* __CERBERUS_COMMAND_HPP__ */
