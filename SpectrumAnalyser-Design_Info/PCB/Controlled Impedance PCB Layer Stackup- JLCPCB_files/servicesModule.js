/**
 * Created by win7 on 2017/10/12.
 */
define(['angular','../../../client/js/common.e91a632e.js'/*,'../../../../client/js/services/mockService'*/], function (angular,common) {
    var appService = angular.module('servicesModule', []);
    /*域名ip*/
     var rootUrl='https://dev-jlcpcb.com:8443/';//开发
    var rootUrl='http://dev-jlcpcb.com:9000/';//开发
    // var rootUrl = "http://192.168.2.204:9000";//谢张泉
    var rootUrl='';//开发
    /*接口定义*/
    appService.factory('httpApi', function() {
        var factory={
            rootUrl:rootUrl,//根域名
            userInfo:'/newCustomer/customerInfo',//用户信息
            lang: '/countryExchangeRate/list',//币种汇率列表
            //orderList:'/newOrder/myOrderList',//订单列表
            orderList:'/newOrder/myOrderBatchList',//订单列表(新)
            orderDetails:'/newOrder/myOrderDetail',//订单详情
            deleteList:'/newOrder/dropOrder',//删除订单
            orderWipProcess:'/newOrder/orderWipProcess',//订单生产进度
            recordOrderListStyle:'/newOrder/recordOrderListStyle',//记录客户中心订单列表风格接口
            dropOrderByBatchNum:'/newOrder/dropOrderByBatchNum',//删除批次订单
            dropOrderByBatchNumAndOrderNum:'/newOrder/delSingleForPreviewBeforePay', //删除单个订单
            saveReorder:'/newOrder/saveReorder',//返单
            cancelOrderByBatchNum:'/newOrder/updateOrderCancel',//取消订单
            checkBatchNumBinding:'/newOrder/checkBatchNumBinding',//判断订单是否支持绑定

            getWebSourceSwitch:'/webContent/getWebSourceSwitch',//判断WEB资源是否可用
            getWebNotice:'/webContent/getWebNotice',//获取通知

            afterSaleSubmit:'/customerComplain/publish',//售后反馈提交
            afterSaleList:'/customerComplain/showComplainList',//售后反馈列表
            replySubmit:'/customerComplain/reply',//客户回复
            updateSatisfaction:'/customerComplain/updateSatisfaction',//满意度提交
            uploadFile:'/fileUploadAttach/upload',//文件上传

            cartList:'/newShoppingCart/showShoppingCart',//购物车列表
            deleteCart:'/newShoppingCart/delShoppingCartItems',//删除购物车
            verifyFormCartList:'/newShoppingCart/settlementVerify',//验证购物车表单
            costCartDetails:'/newShoppingCart/calculationItemCosts',//计算购物车商品价格---修改商品页
            editCartDetails:'/newShoppingCart/editShoppingCartItem',//修改购物车商品---修改商品页
            detailsCartDetails:'/newShoppingCart/getShoppingCartItemDetail',//购物车商品详情---修改商品页
            stencilCountsList:'/newShoppingCart/stencilCountsList',//获取PCB详情页面中PCB QTY可修改的最大数量

            getInitParam:'/newOrder/getInitParam',//获取下单页面初始化参数
            getPcbOrderValidateLimit:'/orderLimit/getPcbOrderValidateLimit',//pcb参数限制
            uploadGerber:'/newOrder/uploadGerber',//gerber文件上传
            analysisGerber:'/newOrder/analysisGerber',//gerber文件预审结果获取
            addOrder:'/newOrder/addOrder',//生成订单,加入购物车
            cartCheckState:'/newShoppingCart/cartCheckState',//判断是否可以修改购物车订单
            queryAchieveDate:'/newShoppingCart/queryAchieveDate',//获取所有交期

            addressList:'/userAddress/list',//地址列表
            deleteAddress:'/userAddress/delAddress',//地址列表删除
            addAddress:'/userAddress/addAddress',//地址列表添加
            setDefaultAddress:'/userAddress/setDefault',//地址列表设置默认
            updateAddress:'/userAddress/updateAddress',//地址列表修改
            getAddressByUUID:'/userAddress/getAddressByUUID' ,//地址列表根据传过来的id渲染数据
            countryRecordList:'/userAddress/countryRecordList' ,//地址列表国家渲染数据

            submitCartList:'/orderTrade/settlementItemFromShoppingCart',//提交购物车
            calculateCostForBinding:'/orderTrade/calculateCostForBinding',//重新计算结算价格信息
            componentsList:'/orderTrade/queryCanBeBoundComponentsList',//获取可绑定的元器件信息列表
            queryCanBindPcbList:'/orderTrade/queryCanBindPcbList',//获取可绑定的PCB信息列表
            queryShippingMethodList:'/orderTrade/queryShippingMethodList',//根据国家获取可用运输方式列表
            payOrder:'/orderTrade/payDirectly',//打包结算信息（先付后审）
            packagingForReview:'/orderTrade/packagingForReview',//打包结算信息（先审后付）
            payAfterReview:'/orderTrade/payAfterReview',//支付（先审后付--审核后支付）
            initPayAfterReviewTradeInfo:'/orderTrade/initPayAfterReviewTradeInfo',//结算页面初始化参数（先审后付后支付）
            initPayDirectlyInfoByBatchNum:'/orderTrade/initPayDirectlyInfoByBatchNum',//结算页面初始化参数（先付后审，支付失败后重新支付）
            initSupplementTradeInfo:'/orderTrade/initSupplementTradeInfo',//补款页面初始化参数（补款）
            paySupplement:'/orderTrade/paySupplement',//补款支付（补款）
            selectAddress:'/orderTrade/selectAddress',//获取收货地址，账单地址
            selectCountryList:'/orderTrade/selectCountryList',//获取国家列表（增、改、查收货地址时使用）

            getTracking:'/express/getTrackingContent',//获取物流轨迹信息

            customerFlag: '/customerFlagApi/list',//国家
            changeShip: '/customerFlagApi/change/ship',//修改发货方式
            changeCode: '/customerFlagApi/change',//修改货币
            /*发票页*/
            invoiceList: '/newOrder/invoice',/*发票*/
            /*绑定订单*/
            calculateBindingMoney: '/orderTrade/calculateBindingMoney',/*根据被绑定的批次 和 勾选的订单 计算得到预计的运费、商品费和总费用*/
            packagingForReviewBinding: '/orderTrade/packagingForReviewBinding',/*绑定先审后付单*/

            selectSteelOrderWipProcess: '/newOrder/selectSteelOrderWipProcess',/*钢网订单生产进度接口*/
            calculate: '/ecp/impedance/calculate', /*阻抗神器接口*/

            /*支付*/
            balanceList: "/balance/balanceList",//查询账单记录
            queryJpayBalance: "/balance/queryJpayBalance",//余额交易记录
            queryJpayRecharge: "/balance/queryJpayRecharge",//余额充值申请记录
            checkPassword: "/balance/checkPassword",//校验密码是否正确
            getPublicKey: "/balance/getPublicKey",//获取支付密码加密的公钥
            getToBank: "/balance/getToBank",//获取充值银行信息
            queryJpayAccountInfo: "/balance/queryJpayAccountInfo",//查询jpay账户是否激活 和账户余额
            submitApply: "/balance/submitApply",//提交充值申请
            uploadApplyFile: "/balance/uploadApplyFile",//上传充值申请文件
            getOrderStatus: "/orderTrade/getOrderStatus",//获取订单状态，判断Jpay返回的消息是否消费完成

            /*Jpay激活*/
            queryCustomerInfo:"/jpayAccount/queryCustomerInfo",//获取当前用户信息
            sendCustomerJpayEmail:"/jpayAccount/sendCustomerJpayEmail",//激活jpay帐号,发送验证邮箱
            saveJpayAccount:"/jpayAccount/saveJpayAccount",//帐号激活,保存支付密码
            resetJpayAccountPassword: "/jpayAccount/resetJpayAccountPassword",//重置JPAY账户密码
            isOpenJpayAccount: "/jpayAccount/isOpenJpayAccount",//是否开启jpay账户(是:yes; 否:no
            /*动态广告*/
            dynamicAdvertising:"/webContent/getAdNotices", //获取动态广告信息
            /*优惠券*/
            getCoupons:"/coupons/getCoupons",//客户所有优惠券
            getAvailableCoupons:"/coupons/getAvailableCoupons",//查询可用优惠券
            checkCouponsCode:"/coupons/checkCouponsCode" ,//检查优惠码是否可用
            /*绑定订单获取超时时间*/
            combineOrderTime:"/newOrder/selectOrderCancelTime",//获取绑定订单支付超时还剩多少时间
        };
        return factory;
    });
    /*封装get/post请求*/
    appService.factory('myServer', function($http, $q) {
        var factory={};
        /*get*/
        factory.getList=function(url){
            postCfg = {
                withCredentials: true
            };
            var defer = $q.defer();
            $http.get(rootUrl+url,postCfg).success(function (data) {
                defer.resolve(data);
            }).error(function (data) {
                defer.reject(data);
            });
            return defer.promise;
        };
        /*获取html文本*/
        factory.getContent=function(url){
            postCfg = {
                withCredentials: true
            };
            var defer = $q.defer();
            $http.get(url,postCfg).success(function (data) {
                defer.resolve(data);
            }).error(function (data) {
                defer.reject(data);
            });
            return defer.promise;
        };
        /*post*/
        factory.postList=function(url,params){
            postCfg = {
                withCredentials: true,
                headers: { 'Content-Type': 'application/x-www-form-urlencoded'},
                transformRequest: function (params) {
                    return $.param(params);
                }
            };
            var defer = $q.defer();
            $http.post(rootUrl+url,params,postCfg).success(function (data) {
                defer.resolve(data);
            }).error(function (data) {
                if(data.status==203){
                   // console.log(data)
                }
                defer.reject(data);
            });
            return defer.promise;
        };
        /*post*/
        factory.jsonPost=function(url,params){
            postCfg = {
                withCredentials: true,
            };
            var defer = $q.defer();
            $http.post(rootUrl+url,params,postCfg).success(function (data) {
                defer.resolve(data);
            }).error(function (data) {
                if(data.status==203){
                   // console.log(data)
                }
                defer.reject(data);
            });
            return defer.promise;
        };
        return factory;
    });
    /*请求接口二次封装*/
    appService.service('httpService',function(myServer,$timeout){
        //get方式
        this.getApi = function(url,successCallback){
            myServer.getList(url)
                .then(function(data) {
                      //  console.log(url);
                      //  console.log(data);
                        successCallback(data);
                    },
                function(rejection) {
                    $timeout(function(){
                        common.noticeModal("The network is busy. Please try again later."+rejection.config.url+" status:"+rejection.status);
                    },0);
                   // console.log("请求错误");
                });
        };
        //    post方式
        this.postApi = function(url,params,successCallback){
            myServer.postList(url,params)
                .then(function(data) {
                        //console.log(url);
                       // console.log(data);
                        successCallback(data);
                    },
                    function (rejection) {
                        $timeout(function () {
                            common.noticeModal("The network is busy. Please try again later."+rejection.config.url+" status:"+rejection.status);
                        }, 0);
                        // console.log("请求错误");
                    });
        };
        //    jsonpost方式
        this.jsonPostApi = function (url, params, successCallback) {
            myServer.jsonPost(url, params)
                .then(function (data) {
                        //console.log(url);
                        // console.log(data);
                        successCallback(data);
                    },
                    function (rejection) {
                        $timeout(function () {
                            common.noticeModal("The network is busy. Please try again later."+rejection.config.url+" status:"+rejection.status);
                        }, 0);
                        // console.log("请求错误");
                    });
        };
    });
    appService.service('GoogleTagManager', function($window) {
        this.push = function(data) {
            try {
                $window.dataLayer.push(data);
            } catch (e) {}
        };
    });
    appService.service('TwitterTag', function () {

        this.loadTwitter = function () {

            !function (e, t, n, s, u, a) {
                e.twq || (s = e.twq = function () {
                    s.exe ? s.exe.apply(s, arguments) : s.queue.push(arguments);
                }, s.version = '1.1', s.queue = [], u = t.createElement(n), u.async = !0, u.src = '//static.ads-twitter.com/uwt.js',
                    a = t.getElementsByTagName(n)[0], a.parentNode.insertBefore(u, a))
            }(window, document, 'script');
// Insert Twitter Pixel ID and Standard Event data below
            twq('init', 'o14em');
            twq('track', 'PageView');
        };
    });
    return appService;
});