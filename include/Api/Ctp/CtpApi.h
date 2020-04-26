// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_INCLUDE_API_CTP_CTPAPI_H_
#define FT_INCLUDE_API_CTP_CTPAPI_H_

#include <memory>
#include <string>
#include <vector>

#include "Api/Ctp/CtpMdApi.h"
#include "Api/Ctp/CtpTradeApi.h"
#include "GeneralApi.h"

namespace ft {

class CtpApi : public GeneralApi {
 public:
  explicit CtpApi(EventEngine* engine)
    : GeneralApi(engine) {
    trade_api_.reset(new CtpTradeApi(this));
    md_api_.reset(new CtpMdApi(this));
  }

  bool login(const LoginParams& params) override {
    if (!params.md_server_addr().empty()) {
      if (!md_api_->login(params))
        return false;
    }

    if (!params.front_addr().empty()) {
      if (!trade_api_->login(params))
        return false;
    }

    return true;
  }

  bool logout() override {
    bool ret = true;

    if (!md_api_->logout())
      ret = false;

    if (!trade_api_->logout())
      ret = false;

    return ret;
  }

  std::string send_order(const Order* order) override {
    return trade_api_->send_order(order);
  }

  bool cancel_order(const std::string& order_id) override {
    return trade_api_->cancel_order(order_id);
  }

  bool query_contract(const std::string& ticker) override {
    return trade_api_->query_contract(ticker);
  }

  bool query_position(const std::string& ticker) override {
    return trade_api_->query_position(ticker);
  }

  bool query_account() override {
    return trade_api_->query_account();
  }

  bool query_margin_rate(const std::string& ticker) override {
    return trade_api_->query_margin_rate(ticker);
  }

 private:
  std::unique_ptr<CtpTradeApi>  trade_api_;
  std::unique_ptr<CtpMdApi>     md_api_;
};

}  // namespace ft

#endif  // FT_INCLUDE_API_CTP_CTPAPI_H_