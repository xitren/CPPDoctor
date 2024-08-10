/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/
#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/exceptions.hpp>
#include <xitren/parsers/errors/locator.hpp>

#include <fmt/core.h>

#include <fstream>
#include <future>
#include <list>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

namespace xitren::parsers::errors {

class locator_pipe : public comm::observer<std::string>, public comm::observable<std::list<std::string>> {
    using data_type   = std::list<std::list<std::string>>;
    using sync_type   = std::mutex;
    using err_type    = std::runtime_error;
    using treads_type = std::list<std::shared_ptr<std::jthread>>;

public:
    locator_pipe() = default;
    ~locator_pipe() override
    {
        join();
    }

    locator_pipe&
    operator=(locator_pipe const& other)
        = delete;
    locator_pipe&
    operator=(locator_pipe const&& other)
        = delete;
    locator_pipe(locator_pipe const& val) = delete;
    locator_pipe(locator_pipe&& val)      = delete;

    void
    join() const {
        for (auto const& thread : treads_) {
            if (thread->joinable()) {
                thread->join();
            }
        }
    }

    void
    data(void const* /*src*/, std::string const& nd) final
    {
        using namespace std::literals;
        if (error_ != nullptr) {
            throw broken_pipe("Pipe was broken! "s + error_->what());
        }
        auto thread = std::make_shared<std::jthread>(
            [this](std::string const& name) {
                try {
                    locator data(name);
                    for (auto const& item : data) {
                        std::lock_guard lock{lock_};
                        this->notify_observers(item);
                    }
                } catch (std::system_error& err) {
                    error_
                        = std::make_shared<err_type>(fmt::format("{} Error code : {}", err.what(), err.code().value()));
                } catch (std::exception& err) {
                    error_ = std::make_shared<err_type>(fmt::format("{}", err.what()));
                }
            },
            nd);
        treads_.push_back(thread);
    }

private:
    sync_type                 lock_{};
    std::shared_ptr<err_type> error_{nullptr};
    treads_type               treads_{};
};

}    // namespace xitren::parsers::errors
