/**
 * Created by win7 on 2017/10/12.
 */
/**
 * routes.js路由模块
 */
define(["../../client/js/app.9566a3b7.js?ver=2",'../../client/js/common.e91a632e.js'], function (app,common) {
    //app是Angular应用对象
    app.config(
        function ($routeProvider,$httpProvider,$compileProvider) {
            /*动态链接---unsafe*/
            $compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|javascript?|ftp|mailto|chrome-extension):/);
            $routeProvider
                .when('/order', {//订单列表
                    templateUrl: '../client/template/mine/order.fbfb002d.html?ver=3',
                    controller: 'orderCtrl',
                    controllerUrl: '../client/js/controllers/mine/orderCtrl.1c89b156.js'
             /*       dependencies: ['directive/orderDirective']*/
                })
                .when('/orderDetails', {//订单详情
                    templateUrl: '../client/template/mine/orderDetails.62e1491d.html',
                    controller: 'orderDetailsCtrl',
                    controllerUrl: '../client/js/controllers/mine/orderDetailsCtrl.d320f698.js'
                })
                .when('/payMethod', {//支付方式
                    templateUrl: '../client/template/payment/payMethod.53f49d4b.html',
                    controller: 'payMethodCtrl',
                    controllerUrl: '../client/js/controllers/payment/payMethodCtrl.1d35c97c.js'
                })
                .when('/paySave', {//先审后付（保存订单）
                    templateUrl: '../client/template/payment/paySave.c827d3fe.html',
                    controller: 'paySaveCtrl',
                    controllerUrl: '../client/js/controllers/payment/paySaveCtrl.0174e3e7.js'
                })
                .when('/payResult', {//支付结果
                    templateUrl: '../client/template/payment/payResult.892d54c0.html',
                    controller: 'payResultCtrl',
                    controllerUrl: '../client/js/controllers/payment/payResultCtrl.cd534971.js'
                })
                .when('/paypalReview', {//paypal支付查看
                    templateUrl: '../client/template/payment/paypalReview.dff67cba.html',
                    controller: 'paypalReviewCtrl',
                    controllerUrl: '../client/js/controllers/payment/paypalReviewCtrl.85303aa6.js'
                })
                .when('/paypalReturn', {//paypal支付结果
                    templateUrl: '../client/template/payment/paypalReturn.6a239ba6.html',
                    controller: 'paypalReturnCtrl',
                    controllerUrl: '../client/js/controllers/payment/paypalReturnCtrl.5155aa80.js'
                })
                .when('/supplement', {//补款页
                    templateUrl: '../client/template/payment/supplement.b4972d62.html',
                    controller: 'supplementCtrl',
                    controllerUrl: '../client/js/controllers/payment/supplementCtrl.8150d3f1.js'
                })
                .when('/afterSale', {//售后服务
                    templateUrl: '../client/template/mine/afterSale.4815c127.html',
                    controller: 'afterSaleCtrl',
                    controllerUrl: '../client/js/controllers/mine/afterSaleCtrl.82562f22.js'
                })
                .when('/afterSaleResult', {//售后服务结果
                    templateUrl: '../client/template/mine/afterSaleResult.748915b0.html',
                    controller: 'afterSaleResultCtrl',
                    controllerUrl: '../client/js/controllers/mine/afterSaleResultCtrl.1443665c.js'
                })
                .when('/cart', {//购物车
                    templateUrl: '../client/template/shoppingCart/cart.49321268.html?ver=2',
                    controller: 'cartCtrl',
                    controllerUrl: '../client/js/controllers/shoppingCart/cartCtrl.5eee759c.js'
                })
                .when('/cartEdit', {//购物车修改页
                    templateUrl: '../client/template/shoppingCart/cartEdit.92299994.html',
                    controller: 'cartEditCtrl',
                    controllerUrl: '../client/js/controllers/shoppingCart/cartEditCtrl.18e6394d.js'
                })
                .when('/quote', {//下单页
                    templateUrl: '../client/template/placeOrder/quote.8cf6be36.html',
                    controller: 'quoteCtrl',
                    controllerUrl: '../client/js/controllers/placeOrder/quoteCtrl.969c59f4.js'
                })
                .when('/address', {//地址页
                    templateUrl: '../client/template/mine/address.74f3d2ad.html',
                    controller: 'addressCtrl',
                    controllerUrl: '../client/js/controllers/mine/addressCtrl.a1e2718d.js'
                })
                .when('/addressEdit', {//地址添加、修改页
                    templateUrl: '../client/template/mine/addressEdit.07d5e34f.html',
                    controller: 'addressEditCtrl',
                    controllerUrl: '../client/js/controllers/mine/addressEditCtrl.22548007.js'
                })
                .when('/invoice', {//发票信息
                    templateUrl: '../client/template/mine/invoice.d7768efa.html',
                    controller: 'invoiceCtrl',
                    controllerUrl: '../client/js/controllers/mine/invoiceCtrl.ee3a724f.js'
                })
                .when('/impedance', {//阻焊说明
                    templateUrl: '../client/template/placeOrder/impedance.9c7b60e4.html',
                    controller: 'impedanceCtrl',
                    controllerUrl: '../client/js/controllers/placeOrder/impedanceCtrl.eb849a7c.js'
                })
                .when('/impedanceCalculation', {//阻焊神器
                    templateUrl: '../client/template/placeOrder/impedanceCalculation.4eccf804.html',
                    controller: 'impedanceCalculationCtrl',
                    controllerUrl: '../client/js/controllers/placeOrder/impedanceCalculationCtrl.2762e1fa.js'
                })
                .when('/account', {//账户中心
                    templateUrl: '../client/template/account/account.440f97a2.html',
                    controller: 'accountCtrl',
                    controllerUrl: '../client/js/controllers/account/accountCtrl.bac7a505.js'
                })
                .when('/transactions', {//余额消费
                    templateUrl: '../client/template/account/transactions.45abc8c4.html',
                    controller: 'transactionsCtrl',
                    controllerUrl: '../client/js/controllers/account/transactionsCtrl.f27e841f.js'
                })
                .when('/topUp', {//充值中心
                    templateUrl: '../client/template/account/topUp.ab761edf.html',
                    controller: 'topUpCtrl',
                    controllerUrl: '../client/js/controllers/account/topUpCtrl.00a26e80.js'
                })
                .when('/topUpSuccess', {//充值成功
                    templateUrl: '../client/template/account/topUpSuccess.5434a813.html',
                    controller: 'topUpSuccessCtrl',
                    controllerUrl: '../client/js/controllers/account/topUpSuccessCtrl.4d985fee.js'
                })
                .when('/billing', {//账单列表
                    templateUrl: '../client/template/account/billing.c4585fb3.html',
                    controller: 'billingCtrl',
                    controllerUrl: '../client/js/controllers/account/billingCtrl.2c9b047b.js'
                })
                .when('/account/jpayActive', {//未激活页
                    templateUrl: '../client/template/account/jpayActive.86ec723a.html',
                    controller: 'jpayActiveCtrl',
                    controllerUrl: '../client/js/controllers/account/jpayActiveCtrl.f156248f.js'
                })
                .when('/account/jpayActive/verifyEmail', {//激活流程 验证邮件
                    templateUrl: '../client/template/account/jpayVerifyEmail.369001b0.html',
                    controller: 'jpayVerifyEmailCtrl',
                    controllerUrl: '../client/js/controllers/account/jpayVerifyEmailCtrl.43a72389.js'
                })
                .when('/account/jpayActive/setPassword', {//激活流程 设置支付密码
                    templateUrl: '../client/template/account/jpaySetPassword.0892e443.html',
                    controller: 'jpaySetPasswordCtrl',
                    controllerUrl: '../client/js/controllers/account/jpaySetPasswordCtrl.e21e37b3.js'
                })
                .when('/account/jpayActive/success', {//激活流程 激活成功
                    templateUrl: '../client/template/account/jpayOperationSuccess.1924bc6a.html',
                    controller: 'jpayOperationSuccessCtrl',
                    controllerUrl: '../client/js/controllers/account/jpayOperationSuccessCtrl.9885d34e.js'
                })
                .when('/account/jpayResetPassword/verifyEmail', {//重置密码 验证邮件
                    templateUrl: '../client/template/account/jpayVerifyEmail.369001b0.html',
                    controller: 'jpayVerifyEmailCtrl',
                    controllerUrl: '../client/js/controllers/account/jpayVerifyEmailCtrl.43a72389.js'
                })
                .when('/account/jpayResetPassword/setPassword', {//重置密码 设置支付密码
                    templateUrl: '../client/template/account/jpaySetPassword.0892e443.html',
                    controller: 'jpaySetPasswordCtrl',
                    controllerUrl: '../client/js/controllers/account/jpaySetPasswordCtrl.e21e37b3.js'
                })
                .when('/account/jpayResetPassword/success', {//重置密码 激活成功
                    templateUrl: '../client/template/account/jpayOperationSuccess.1924bc6a.html',
                    controller: 'jpayOperationSuccessCtrl',
                    controllerUrl: '../client/js/controllers/account/jpayOperationSuccessCtrl.9885d34e.js'
                })
                .when('/ourTeam', {//团队介绍
                    templateUrl: '../client/template/ourTeam/ourTeam.afa004cc.html',
                    controller: 'ourTeamCtrl',
                    controllerUrl: '../client/js/controllers/ourTeam/ourTeamCtrl.a1b0357a.js'
                })
                .when('/coupon', {//我的优惠券
                    templateUrl: '../client/template/account/coupon.9537663b.html',
                    controller: 'couponCtrl',
                    controllerUrl: '../client/js/controllers/account/couponCtrl.d4db7a09.js'
                })

                /*  .when('/home', {//首页
                    templateUrl: '../client/template/home/home.html',
                    controller: 'homeCtrl',
                    controllerUrl: '../client/js/controllers/home/homeCtrl.js'
                })
                .when('/bigPic', {//首页产品大图
                    templateUrl: '../client/template/home/bigPic.html',
                    controller: 'homeCtrl',
                    controllerUrl: '../client/js/controllers/home/homeCtrl.js'
                })
                .when('/capabilities', {//属性列表
                    templateUrl: '../client/template/home/capabilities.html',
                    controller: 'capabilitiesCtrl',
                    controllerUrl: '../client/js/controllers/home/capabilitiesCtrl.js'
                })
               */
                .otherwise({ redirectTo: '/order' });
            //Http Intercpetor to check auth failures for xhr requests
            $httpProvider.interceptors.push('authHttpResponseInterceptor');
        });
    app.run(function ($rootScope, $location, GoogleTagManager, TwitterTag) {
        common.loadingModal();
        /* 监听路由的状态变化 */
        $rootScope.$on('$routeChangeStart', function(evt, next, current){
            /*关闭弹框*/
            common.hideModal(".modal");
            /*不需要登录*/
            if (next.$$route.originalPath == "/impedance" || next.$$route.originalPath == "/impedanceCalculation"||next.$$route.originalPath == "/ourTeam"||next.$$route.originalPath == "/quote") {
                window.noLogin=true;
            }else{
                window.noLogin=false;
            }
            /*滚动到顶部*/
            cancelAnimationFrame(timer);
            var timer = requestAnimationFrame(function fn(){
                var oTop = document.body.scrollTop || document.documentElement.scrollTop;
                if(oTop > 0){
                    document.body.scrollTop = document.documentElement.scrollTop = oTop - 50;
                    timer = requestAnimationFrame(fn);
                }else{
                    cancelAnimationFrame(timer);
                }
            });
        });
        $rootScope.$on('$routeChangeSuccess', function(evt, current, previous) {
            //console.log('路由 ：'+$location.path());
            if (current.$$route.originalPath == "/impedance") {
                $("title").html("Controlled Impedance PCB Layer Stackup- JLCPCB");
                $("meta[name='description']").attr("content", "JLCPCB offers High-precision Multilayer PCB with impedance control at no extra charge. Learn about our PCB laminate material and controlled impedance layer stackup here.");
                $("meta[name='keywords']").attr("content", "controlled impedance pcb,Impedance Controlled PCBs,controlled impedance circuit board,impedance control service,controlled impedance stackup");
            } else {
                $("title").html("PCB Prototype & PCB Fabrication Manufacturer - JLCPCB");
                $("meta[name='keywords']").attr("content", "PCB manufacturing, circuit board, PCB fabrication, pcb board, printed circuit board manufacturers, custom circuit board, pcb board, printed circuit board");
                $("meta[name='description']").attr("content", "China's Largest PCB Prototype Manufacturer, offers 24 hours Quick Turn PCB prototype and Reliable small-batch PCB production.");
            }
        });
        $rootScope.$on('$viewContentLoaded', function() {
            /*google*/
            var path= $location.path(),
                absUrl = $location.absUrl(),
                virtualUrl = absUrl.substring(absUrl.indexOf(path));
            GoogleTagManager.push({ event: 'virtualPageView', virtualUrl: virtualUrl });
            /*twitter*/
            TwitterTag.loadTwitter();
        });
    });
    app.factory('authHttpResponseInterceptor', function ($rootScope, $q, $timeout,$routeParams) {
        //拦截器配置
        return {
            request: function (config) {
                return config || $q.when(config);
            },
            response: function(response){
                //请求返回异常类型判
                if (response.status == 203||response.status == 403) {
                    if(!window.noLogin){
                        common.noticeModal("You aren't logged in. <a class='blue' href='/auth/login'>Please login.</a>");
                        window.location.href = '/auth/login';//跳转登录
                    }
                } else if (response.status != 200) {
                    $timeout(function(){
                        common.noticeModal("The network is busy. Please try again later."+response.config.url+" status:"+response.status);
                    },0);
                }
                return response || $q.when(response);
            },
            responseError: function(rejection) {
                //请求返回异常类型判断
                if (rejection.status == 203||rejection.status == 403) {
                    if(!window.noLogin){
                        common.noticeModal("You aren't logged in. <a class='blue' href='/auth/login'>Please login.</a>");
                        window.location.href = '/auth/login';//跳转登录
                    }
                } else if (rejection.status != 200) {
                    $timeout(function(){
                        common.noticeModal("The network is busy. Please try again later."+rejection.config.url+" status:"+rejection.status);
                    },0);
                }
                return $q.reject(rejection);
            }
        };
    })
});