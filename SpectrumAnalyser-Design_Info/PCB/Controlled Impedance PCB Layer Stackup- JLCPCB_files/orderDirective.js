/**
 * Created by win7 on 2017/10/16.
 */
define(["angular","../../../client/js/common.e91a632e.js","../../../client/js/directive/directiveModule.5dc53cec.js"], function (angular,common,directiveModule) {
    /*订单类型切换*/
    directiveModule.directive('orderType', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {
                tabActive:"=",//变量
                tabOrder:"&",//方法
            },
            templateUrl : '../client/template/component/orderType.744934f5.html',
            link:function(scope,element){
            }
        }
    });
    /*订单列表search*/
    directiveModule.directive('search', function() {
        return {
            restrict : 'EA',
            replace : false,
            scope : {
                codeSearch : '&',
                searchOrder : '&',
                searchTxt: '='
            },
            template: ' <div class="input-group ">'+
                '<input ng-model="searchTxt" ng-keypress="codeSearch({event:$event})" type="text" class="form-control input-sm searchTxt" placeholder="Search for...">'+
                '<span class="input-group-btn ">'+
                '<button ng-click="searchOrder()" class="btn btn-default bgSearch input-sm" type="button">&nbsp;</button>'+
                '</span>'+
            '</div>',
            link:function(scope){}
        }
    });
    /*订单列表加载更多*/
    directiveModule.directive('loadMore', function() {
        return {
            restrict : 'EA',
            scope : {
                moreList:"&",
            },
            replace : false,
            template : '<a  ng-click="moreList()" href="javascript:void(0);" class="btn btn-primary w160 radius20  bgJian">more</a>',
        }
    });
    /*个人中心菜单*/
    directiveModule.directive('mineLeftMenu', function() {
        return {
            restrict : 'EA',
            scope : {
                headImg : '@',
                username: '@'
            },
            replace : false,
            templateUrl: '../client/template/component/mineLeftMenu.d49d8b91.html',
            controller: function ($scope, httpService, httpApi, $rootScope, $location) {
                /*是否开启jpay账户(是:yes; 否:no*/
                httpService.getApi(httpApi.isOpenJpayAccount, function (dat) {
                    $scope.isOpenJpayAccount = dat.message;
                    if ($scope.isOpenJpayAccount == 'yes') {
                        /*查询jpay账户是否激活 和账户余额*/
                        httpService.getApi(httpApi.queryJpayAccountInfo, function (data) {
                            $scope.jpayAccountStatus = data.jpayAccountStatus;
                        });
                    }
                });
                $scope.rootUrl=httpApi.rootUrl;//根域名
                /*开关*/
                var setInter=setInterval(function(){
                    if($rootScope.webSourceSwitch){
                        $scope.webSourceSwitch=$rootScope.webSourceSwitch;
                        clearInterval(setInter)
                    }
                },1000);
                $scope.jlcpcbUrl=encodeURIComponent("https://www.jlcpcb.com");

                $scope.currentPath=$location.path();
                if($scope.currentPath.indexOf('address') != -1){
                    $scope.isAddress=true;
                }
                if ($scope.currentPath.indexOf('account') != -1 || $scope.currentPath.indexOf('transactions') != -1 || $scope.currentPath.indexOf('topUp') != -1 || $scope.currentPath.indexOf('topUpSuccess') != -1 || $scope.currentPath.indexOf('billing') != -1|| $scope.currentPath.indexOf('coupon') != -1) {
                    $scope.isAccount = true;
                }
                if ($scope.currentPath.indexOf('account') != -1 || $scope.currentPath.indexOf('transactions') != -1 || $scope.currentPath.indexOf('topUp') != -1 || $scope.currentPath.indexOf('topUpSuccess') != -1) {
                    $scope.isJpay = true;
                }

            },
            link:function(scope,element,attr){}
        }
    });
    /*商品列表box*/
    directiveModule.directive('productBoxList', function() {
        return {
            restrict : 'EA',
            scope : {
                list : '=',//列表
                deleteOrder:'&',//删除方法
                itemActive:"=",//选中的item
            },
            replace : false,
            templateUrl: '../client/template/component/productBoxList.html',
            controller:function($scope,httpService,httpApi){
                $scope.rootUrl=httpApi.rootUrl;//根域名
                /*订单详情弹框*/
                $scope.orderNumber=function(item){
                    if(item.orderType!=3){
                        /*pcb*/
                        if(item.detail.pcbDetail.historyStencilWitdh>0 && item.detail.pcbDetail.historyStencilLength>0){
                            item.detail.pcbDetail.stencilWidth =Math.round( item.detail.pcbDetail.historyStencilWitdh);
                            item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.historyStencilLength);
                        }else{
                            //是否有工艺边
                            if(item.detail.pcbDetail.sestencilCountX>1 || item.detail.pcbDetail.sestencilCountY>1){
                                if(item.detail.pcbDetail.stencilWidth>item.detail.pcbDetail.stencilLength){
                                    item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth-10);
                                }else{
                                    item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.stencilLength-10);
                                }
                            }
                            if(item.detail.pcbDetail.sestencilCountX>0){
                                item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth/item.detail.pcbDetail.sestencilCountX);
                            }
                            if(item.detail.pcbDetail.sestencilCountY>0){
                                item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.stencilLength/item.detail.pcbDetail.sestencilCountY);
                            }
                        }
                    }
                    $scope.itemActive=item;
                    common.showModal("orderDetailsModal");
                };
                /*订单状态弹框*/
                $scope.orderProcess=function(item){
                    httpService.postApi(httpApi.orderWipProcess,{
                        produceOrderNum:item.produceOrderNum,
                        orderStatus:item.orderStatus
                    },function(data){
                        $scope.itemActive=data.list;
                        common.showModal("orderProcessModal");
                        var ind=1;//完成第几步
                        angular.forEach(data.list,function(dat,index){
                            if(dat.beginTime!=null&&dat.beginTime!=''){
                                ind=index+1;
                                if(dat.technicsProcessId=="40"){
                                    ind=data.list.length;
                                    return;
                                }
                            }
                        });

                        if(data.list.length>0){
                            var hasParam=(ind/data.list.length*100).toFixed(0);//完成百分比
                        }else{
                            var hasParam=0;
                        }
                        /*圆形进度条*/
                        var myChart = echarts.init(document.getElementById('main'));
                        var dataStyle = {
                            normal: {
                                color: '#ddd',
                                label: {show:false},
                                labelLine: {show:false}
                            }
                        };
                        var placeHolderStyle = {
                            normal : {
                                color: '#6AB6FF',
                                label: {show:false},
                                labelLine: {show:false}
                            }
                        };
                        var option = {
                            title: {
                                text: hasParam+'%',
                                x: 'center',
                                y: 'center',
                                itemGap: 20,
                                textStyle : {
                                    color : '#6AB6FF',
                                    fontFamily : '微软雅黑',
                                    fontSize : 35,
                                    fontWeight : 'bolder'
                                }
                            },
                            series : [
                                {
                                    name: '1',
                                    type: 'pie',
                                    clockWise: false,
                                    radius: [60, 75],
                                    itemStyle: dataStyle,
                                    data: [
                                        {
                                            value: 100-hasParam,
                                            name: 'process'
                                        },
                                        {
                                            value: hasParam,
                                            name: 'invisible',
                                            itemStyle: placeHolderStyle
                                        }
                                    ]
                                }
                            ]
                        };
                        myChart.setOption(option);
                    });
                };
                /*订单支付，form表单提交*/
                $scope.submit=function(item){
                    var chooseCheckbox =item.orderNum+":"+item.orderType;
                    $("#chooseCheckbox").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/trade");
                    $("#allProjectFrom").submit();
                };
                /*补款订单支付，form表单提交*/
                $scope.fillMoney=function(item){
                    var chooseCheckbox =item.orderSupplementNum;
                    $("#orderSupplementNum").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/supplement/trade");
                    $("#allProjectFrom").submit();
                };
                /*detail弹框提示*/
                $scope.alertNote=function(item){
                    common.alertModal(item.problemRemark);
                }
            },
            link:function(scope){}
        }
    });
    /*商品列表list*/
    directiveModule.directive('productOneList', function() {
        return {
            restrict : 'EA',
            scope : {
                list : '=',//列表
                deleteOrder:'&',//删除方法
                itemActive:"=",//选中的item
                detailHide:"@",
            },
            replace : false,
            templateUrl: '../client/template/component/productOneList.html',
            controller:function($scope,httpService,httpApi){
                $scope.rootUrl=httpApi.rootUrl;//根域名
                /*订单详情弹框*/
                $scope.orderNumber=function(item){
                    if(item.orderType!=3){
                        /*pcb*/
                        if(item.detail.pcbDetail.historyStencilWitdh>0 && item.detail.pcbDetail.historyStencilLength>0){
                            item.detail.pcbDetail.stencilWidth =Math.round( item.detail.pcbDetail.historyStencilWitdh);
                            item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.historyStencilLength);
                        }else{
                            //是否有工艺边
                            if(item.detail.pcbDetail.sestencilCountX>1 || item.detail.pcbDetail.sestencilCountY>1){
                                if(item.detail.pcbDetail.stencilWidth>item.detail.pcbDetail.stencilLength){
                                    item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth-10);
                                }else{
                                    item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.stencilLength-10);
                                }
                            }
                            if(item.detail.pcbDetail.sestencilCountX>0){
                                item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth/item.detail.pcbDetail.sestencilCountX);
                            }
                            if(item.detail.pcbDetail.sestencilCountY>0){
                                item.detail.pcbDetail.stencilLength =Math.round( item.detail.pcbDetail.stencilLength/item.detail.pcbDetail.sestencilCountY);
                            }
                        }
                    }
                    $scope.itemActive=item;
                    common.showModal("orderDetailsModal");
                };
                /*订单状态弹框*/
                $scope.orderProcess=function(item){
                    httpService.postApi(httpApi.orderWipProcess,{
                        produceOrderNum:item.produceOrderNum,
                        orderStatus:item.orderStatus
                    },function(data){
                        $scope.itemActive=data.list;
                        common.showModal("orderProcessModal");
                        var ind=1;//完成第几步
                        angular.forEach(data.list,function(dat,index){
                            if(dat.beginTime!=null&&dat.beginTime!=''){
                                ind=index+1;
                                if(dat.technicsProcessId=="40"){
                                    ind=data.list.length;
                                    return;
                                }
                            }
                        });
                        if(data.list.length>0){
                            var hasParam=(ind/data.list.length*100).toFixed(0);//完成百分比
                        }else{
                            var hasParam=0;
                        }
                        /*圆形进度条*/
                        var myChart = echarts.init(document.getElementById('main'));
                        var dataStyle = {
                            normal: {
                                color: '#ddd',
                                label: {show:false},
                                labelLine: {show:false}
                            }
                        };
                        var placeHolderStyle = {
                            normal : {
                                color: '#6AB6FF',
                                label: {show:false},
                                labelLine: {show:false}
                            }
                        };
                        var option = {
                            title: {
                                text: hasParam+'%',
                                x: 'center',
                                y: 'center',
                                itemGap: 20,
                                textStyle : {
                                    color : '#6AB6FF',
                                    fontFamily : '微软雅黑',
                                    fontSize : 35,
                                    fontWeight : 'bolder'
                                }
                            },
                            series : [
                                {
                                    name: '1',
                                    type: 'pie',
                                    clockWise: false,
                                    radius: [60, 75],
                                    itemStyle: dataStyle,
                                    data: [
                                        {
                                            value: 100-hasParam,
                                            name: 'process'
                                        },
                                        {
                                            value: hasParam,
                                            name: 'invisible',
                                            itemStyle: placeHolderStyle
                                        }
                                    ]
                                }
                            ]
                        };
                        myChart.setOption(option);
                    });
                };
                /*订单支付，form表单提交*/
                $scope.submit=function(item){
                    var chooseCheckbox =item.orderNum+":"+item.orderType;
                    $("#chooseCheckbox").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/trade");
                    $("#allProjectFrom").submit();
                };
                /*补款订单支付，form表单提交*/
                $scope.fillMoney=function(item){
                    var chooseCheckbox =item.orderSupplementNum;
                    $("#orderSupplementNum").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/supplement/trade");
                    $("#allProjectFrom").submit();
                };
                /*detail弹框提示*/
                $scope.alertNote=function(item){
                    common.alertModal(item.problemRemark);
                };
                /*返单锁单提示*/
                $scope.showNotice=function(){
                    common.alertModal("The order has quality problems after sale, and it is not allowed to return the order");
                };
            },
            link:function(scope){}
        }
    });
    /*商品详情列表list*/
    directiveModule.directive('productOneDetail', function() {
        return {
            restrict : 'EA',
            scope : {
                list : '=',//列表
                itemActive:"=",//选中的item
            },
            replace : false,
            templateUrl: '../client/template/component/productOneDetail.f0d221c9.html',
            controller:function($scope,httpService,httpApi,$rootScope){
                $scope.rootUrl=httpApi.rootUrl;//根域名
                /*新列表*/
                $scope.form={
                    selectqtylist:[5,10,15,20,25,30,50,75,100,125,150,200,250,300,400,450,500,600,700,800,900,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6500,7000,7500,8000,25000,30000,40000,50000,60000,70000,80000],//数量
                };
                /*返单数量验证*/
                $scope.checkNumber=function(item){
                    if(item.stencilCountsReorder){
                        item.stencilCountsReorder=item.stencilCountsReorder.toString().replace(/[^\d\.]/g,'');//stencilCountsReorder
                    }
                };
                /*显示返单数量弹框*/
                $scope.showQtylist=function(itemChild){
                    itemChild.qtylist=!itemChild.qtylist;

                };
                /*返单选中数量*/
                $scope.chose=function(listItem,itemChild){
                    itemChild.stencilCountsReorder=listItem;//赋值
                    itemChild.qtylist=false;//隐藏下拉框
                };
                /*返单提交*/
                $scope.reorder=function(event,item){
                    httpService.postApi(httpApi.saveReorder,{
                        produceOrderNum:item.produceOrderNum,
                        stencilCounts:item.stencilCountsReorder
                    },function(data){
                       // console.log(data);
                        if(data==200){
                            /*效果*/
                            var top=event.clientY+(document.documentElement.scrollTop || document.body.scrollTop);
                            var left=event.clientX;
                            $(".reorderSuccess").css({
                                display:"block",
                                top:top+5,
                                left:left,
                                opacity: 1
                            });
                            $(".reorderSuccess").animate({
                                display:"none",
                                top:"70px",
                                left:$(".w1300").offset().left+980,
                                opacity: 'hide'
                            },1000);
                            item.hasOrder=true;//隐藏输入框
                            //common.alertModal('Reorder Success');
                            $rootScope.userInfo.cartNum+=1;//购物车数量加1；
                        }else{
                            common.alertModal('Reorder Failure');
                        }
                    });


                };
                /*返单锁单提示*/
                $scope.showNotice=function(){
                    common.alertModal("The order has quality problems after sale, and it is not allowed to return the order");
                };
                /*订单详情弹框*/
                $scope.orderNumber=function(item){
                    if(item.orderType!=3){
                        /*pcb*/
                        if(item.detail.pcbDetail.historyStencilWitdh>0 && item.detail.pcbDetail.historyStencilLength>0){
                            item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.historyStencilWitdh);
                            item.detail.pcbDetail.stencilLength = Math.round(item.detail.pcbDetail.historyStencilLength);
                        }else{
                            //是否有工艺边
                            if(item.detail.pcbDetail.sestencilCountX>1 || item.detail.pcbDetail.sestencilCountY>1){
                                if(item.detail.pcbDetail.stencilWidth>item.detail.pcbDetail.stencilLength){
                                    item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth-10);
                                }else{
                                    item.detail.pcbDetail.stencilLength =Math.round( item.detail.pcbDetail.stencilLength-10);
                                }
                            }
                            if(item.detail.pcbDetail.sestencilCountX>0){
                                item.detail.pcbDetail.stencilWidth = Math.round(item.detail.pcbDetail.stencilWidth/item.detail.pcbDetail.sestencilCountX);
                            }
                            if(item.detail.pcbDetail.sestencilCountY>0){
                                item.detail.pcbDetail.stencilLength =Math.round( item.detail.pcbDetail.stencilLength/item.detail.pcbDetail.sestencilCountY);
                            }
                        }
                    }
                    $scope.itemActive=item;
                    common.showModal("orderDetailsModal");
                };
                /*订单状态弹框*/
                $scope.orderProcess=function(item){
                    if(item.orderType!=3){
                        httpService.postApi(httpApi.orderWipProcess,{
                            produceOrderNum:item.produceOrderNum,
                            orderStatus:item.orderStatus
                        },function(data){
                            $scope.itemActive=data.list;
                            common.showModal("orderProcessModal");
                            var ind=1;//完成第几步
                            angular.forEach(data.list,function(dat,index){
                                if(dat.beginTime!=null&&dat.beginTime!=''){
                                    ind=index+1;
                                    if(dat.technicsProcessId=="40"){
                                        ind=data.list.length;
                                        return;
                                    }
                                }
                            });
                            if(data.list.length>0){
                                var hasParam=(ind/data.list.length*100).toFixed(0);//完成百分比
                            }else{
                                var hasParam=0;
                            }
                            /*圆形进度条*/
                            var myChart = echarts.init(document.getElementById('main'));
                            var dataStyle = {
                                normal: {
                                    color: '#ddd',
                                    label: {show:false},
                                    labelLine: {show:false}
                                }
                            };
                            var placeHolderStyle = {
                                normal : {
                                    color: '#6AB6FF',
                                    label: {show:false},
                                    labelLine: {show:false}
                                }
                            };
                            var option = {
                                title: {
                                    text: hasParam+'%',
                                    x: 'center',
                                    y: 'center',
                                    itemGap: 20,
                                    textStyle : {
                                        color : '#6AB6FF',
                                        fontFamily : '微软雅黑',
                                        fontSize : 35,
                                        fontWeight : 'bolder'
                                    }
                                },
                                series : [
                                    {
                                        name: '1',
                                        type: 'pie',
                                        clockWise: false,
                                        radius: [60, 75],
                                        itemStyle: dataStyle,
                                        data: [
                                            {
                                                value: 100-hasParam,
                                                name: 'process'
                                            },
                                            {
                                                value: hasParam,
                                                name: 'invisible',
                                                itemStyle: placeHolderStyle
                                            }
                                        ]
                                    }
                                ]
                            };
                            myChart.setOption(option);
                        });
                    }
                };
                /*钢网进度弹框*/
                $scope.selectSteelOrderWipProcess=function (item) {
                    $scope.itemActive=null;
                    /*钢网*/
                    httpService.postApi(httpApi.selectSteelOrderWipProcess,{
                        uuid:item.produceOrderNum,
                        orderStatus:item.orderStatus
                    },function(data){
                        $scope.itemActive=data.steelList;
                        common.showModal("orderSteelProcessModal");
                    });
                };
                /*订单支付，form表单提交*/
                $scope.submit=function(item){
                    var chooseCheckbox =item.orderNum+":"+item.orderType;
                    $("#chooseCheckbox").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/trade");
                    $("#allProjectFrom").submit();
                };
                /*补款订单支付，form表单提交*/
                $scope.fillMoney=function(item){
                    var chooseCheckbox =item.orderSupplementNum;
                    $("#orderSupplementNum").val(chooseCheckbox);
                    $("#allProjectFrom").attr("action",$scope.rootUrl+"/order/supplement/trade");
                    $("#allProjectFrom").submit();
                };
                /*detail弹框提示*/
                $scope.alertNote=function(item){
                    common.alertModal(item.problemRemark);
                }
            },
            link:function(scope){}
        }
    });
    /*订单详情弹框*/
    directiveModule.directive('orderDetails', function() {
        return {
            restrict : 'A',
            scope : {
                list : '=',
                achieveDate:"="
            },
            replace : false,
            templateUrl: '../client/template/component/orderDetails.b7472079.html'
         /*   link:function(scope,element){}*/
        }
    });
    /*PCB订单生产进度弹框*/
    directiveModule.directive('orderProce', function() {
        return {
            restrict : 'A',
            scope : {
                list : '='
            },
            replace : false,
            templateUrl: '../client/template/component/orderProce.ab0149eb.html',
            controller: function ($scope, httpService, httpApi, $rootScope) {
                $scope.videoModal = false;
                $scope.showClose=false;
                $scope.videoUrl = null;
                $scope.showVideoModal = function (index) {
                    switch (index + 1) {
                        case 1:
                            $scope.videoUrl = "/video/2.Drilling.mp4";
                            break;
                        case 2:
                            $scope.videoUrl = "/video/2.Drilling.mp4";
                            break;
                        case 3:
                            $scope.videoUrl = "/video/3.CopperDeposition.mp4";
                            break;
                        case 4:
                            $scope.videoUrl = "/video/4. Imagetheouterlayers.mp4";
                            break;
                        case 5:
                            $scope.videoUrl = "/video/5.PatternPlating.mp4";
                            break;
                        case 6:
                            $scope.videoUrl = "/video/6.AutomaticOpticalInspection(AOI).mp4";
                            break;
                        case 7:
                            $scope.videoUrl = "/video/7.SolderMask.mp4";
                            break;
                        case 8:
                            $scope.videoUrl = "/video/8.Silkscreen.mp4";
                            break;
                        case 9:
                            $scope.videoUrl = "/video/9.HotAirSolderLeveling.mp4";
                            break;
                        case 10:
                            $scope.videoUrl = "/video/10.ElectricalTest.mp4";
                            break;
                        case 11:
                            $scope.videoUrl = "/video/11.Profiling,V-cutscoring.mp4";
                            break;
                        case 12:
                            $scope.videoUrl = "/video/12.Finalinspection-QC.mp4";
                            break;
                        case 13:
                            $scope.videoUrl = "/video/13.Packaging,Delivery.mp4";
                            break;
                    }
                    $scope.videoModal = true;
                }
                $scope.hideModal = function () {
                    $scope.videoModal = false;
                }
                $("#orderProcessModal").on("hidden.bs.modal",function(){
                    $scope.videoModal = false;
                });
            },
            link: function (scope, element) {
            }
        }
    });
    /*钢网订单生产进度弹框*/
    directiveModule.directive('orderSteelProce', function() {
        return {
            restrict : 'A',
            scope : {
                list : '=',
            },
            replace : false,
            templateUrl: '../client/template/component/orderSteelProce.dd024555.html',
            controller:function($scope,httpService,httpApi){
                $scope.form={
                    showProcess:0,
                }
                $scope.$watch("list", function (newVal, oldVal) {
                    /*重新获取快递费用*/
                    if($scope.list&&$scope.list.length>0) {
                        $scope.itemSteelShow=$scope.list[0];$scope.form.showProcess=0;
                    }else{
                        $scope.itemSteelShow=null;
                    }
                });
                $scope.$watch("form.showProcess", function (newVal, oldVal) {
                    /*重新获取快递费用*/
                    if($scope.list&&$scope.list.length>0)  $scope.itemSteelShow=$scope.list[$scope.form.showProcess];
                });


                /*修改进度显示数组*/
             /*   $scope.changeProcess=function(){
                    console.log(22,$scope.form.showProcess)
                    $scope.itemSteelShow=$scope.list[$scope.form.showProcess];
                };*/
            },
            /*   link:function(scope,element){}*/
        }
    });
    /*订单物流进度弹框*/
    directiveModule.directive('orderTracking', function() {
        return {
            restrict : 'A',
            scope : {
                list : '='
            },
            replace : false,
            templateUrl: '../client/template/component/orderTracking.34395ac2.html'
            /*   link:function(scope,element){}*/
        }
    });
    /*list列表切换按钮*/
    directiveModule.directive('oneListTab', function() {
        return {
            restrict : 'EA',
            scope : {
                bool : '='
            },
            replace : false,
            template : ' <a href="javascript:void(0);" class="tabList bgLi" ng-class="{active:bool==1}" ng-click="tabList()"></a>',
            controller:function($scope,httpService,httpApi){
                $scope.tabList=function(){
                    $scope.bool=1;//风格类型
                    $scope.active='active';//类变量
                    httpService.postApi(httpApi.recordOrderListStyle, {
                        orderStyleType: $scope.bool
                    }, function (data) {})
                };
            },
            link:function(scope){}
        }
    });
    /*box列表切换按钮*/
    directiveModule.directive('boxListTab', function() {
        return {
            restrict : 'EA',
            scope : {
                bool : '='
            },
            replace : false,
            template : '<a href="javascript:void(0);" class="tabList bgList" ng-class="{active:bool==0}" ng-click="tabList()"></a>',
            controller:function($scope,httpService,httpApi){
                $scope.tabList=function(){
                    $scope.bool=0;//风格类型
                    $scope.active='active';//类变量
                    httpService.postApi(httpApi.recordOrderListStyle, {
                        orderStyleType: $scope.bool
                    }, function (data) {})
                };
            },
            link:function(scope){}
        }
    });
    /*订单状态*/
    directiveModule.directive('orderStatus', function() {
        return {
            restrict : 'EA',
            scope : {
                orderInfo : '=',
            },
            replace : true,
            templateUrl: '../client/template/component/orderStatus.528f1dd3.html',
            link:function(scope,element){}
        }
    });
    /*钢网尺寸弹框*/
    directiveModule.directive('steelModal', function() {
        return {
            restrict : 'A',
            replace : true,
            templateUrl: '../client/template/component/steelModal.aa7165c9.html'
        }
    });
    /*customerInfo支付方式--地址列表*/
    directiveModule.directive('customerInfo', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {
                list:"=",//地址列表
                formModel:"=",//form
                model:"=",//input
                deleteAddr:"&"//地址删除
            },
            transclude : true,
            templateUrl : '../client/template/component/customerInfo.a622e824.html',
            controller:function($scope,httpService,httpApi){
                /*编辑*/
                $scope.edit=function(item){
                    common.showModal('addressModal');
                    if (item.type == 'shipping') {
                        $scope.formModel.addressType = 'Edit shipping address';
                    } else {
                        $scope.formModel.addressType = 'Edit billing address';
                    }
                    $scope.formModel.uuid=item.uuid;
                    $scope.formModel.firstName=item.firstName;
                    $scope.formModel.lastName=item.lastName;
                    $scope.formModel.companyName=item.companyName;
                    $scope.formModel.street=item.street;
                    $scope.formModel.street2=item.street2;
                    $scope.formModel.city=item.city;
                    $scope.formModel.country=item.country;
                    $scope.formModel.countryName=item.countryName;
                    $scope.formModel.state=item.state;
                    $scope.formModel.postcode=item.postcode;
                    $scope.formModel.phone=item.phone;
                    $scope.formModel.type=item.type;
                    $scope.formModel.borderRed = false;//是否显示红色边框
                    $scope.formModel.borderRed1 = false;//是否显示红色框
                    $scope.formModel.continue=false;//点击continue
                };
            }
        }
    });
    /*customerInfo支付方式添加地址*/
    directiveModule.directive('addAddress', function() {
        return {
            restrict : 'EA',
            replace : false,
            scope : {
                addressModal:'&'
               // formModel:"="
            },
            template: '<a class="blue line35" href="javascript:void(0);">+Add new shipping address</a>',
            link:function(scope,element){
                element.on("click",function(){
                    scope.addressModal();
                })
            }
        }
    });
    /*支付方式datSecurity*/
    directiveModule.directive('datSecurity', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {},
            template : '  <div class="h340 huiBox">'+
                            ' <div class="strong font16 mb20"><span class="icon-bg icon-lock mr10"></span>Data security</div>'+
                            ' <p class="color8 line25">JLCPCB.com uses Secure Sockets Layer(SSL) technology to provide you with the safest, most secure purchase experience possible. SSL technology enables encryption of sensitive information, including passwords and credit card numbers, during your online transactions. All of the forms on our site are secured with SSL technology so your personal information stays safe and out of malicious hands.</p>'+
                            ' <div class="text-right mt20"><span class="icon-bg icon-ssl"></span></div>'+
                    ' </div>'
        }
    });
    /*支付方式datHelp*/
    directiveModule.directive('datHelp', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {},
            template : '  <div class="h340 huiBox">'+
                ' <div class="strong font16 mb20"><span class="icon-bg icon-ques mr10"></span>Need Help?</div>'+
                ' <p><a class="color8" href="">How to make a payment?</a></p>'+
                ' <p><a class="color8" href="">Is my payment information safe?</a></p>'+
                ' <p><a class="color8" href="">Do you offer global shipping?</a></p>'+
                ' <div class="strong font16 mt30 mb20"><span class="icon-bg icon-email mr10"></span>Contact Us?</div>'+
                ' <p><a class="color8" href="">support@jlcpcb.com</a></p>'+
            ' </div>'
        }
    });
    /*绑定支付事件*/
    directiveModule.directive('payClick', function() {
        return {
            restrict : 'A',
            priority:1,
            scope:{
                pay:"&"
            },
            link:function(scope,elem,attr){
                elem.on("click",function(){
                    scope.pay();
                })
            }
        }
    });
    /*修改地址保存事件*/
    directiveModule.directive('addressClick', function() {
        return {
            restrict : 'A',
            priority:1,
            scope:{
              saveAddr:"&"
            },
            link:function(scope,elem,attr){
                elem.on("click",function(){
                    scope.saveAddr();
                })
            }
        }
    });
    /*配送方式*/
    directiveModule.directive('shippingMethod', function() {
        return {
            restrict : 'EA',
            replace : true,
            templateUrl:'../client/template/component/shippingMethod.6e68fa65.html',
            /*          template: '   <div class="plr20 ptb20">'+
                          '  <div class="line20 mb20" ng-repeat="item in setResponse.effectiveExpressList">'+
                          ' <label class="normal mb0"><input ng-disabled="form.bindPcb" ng-click="chose(item)" ng-model="form.shippingMethod" value="{{item.expressName}}" class="ver mt0" type="radio" name="shipping">' +
                          ' <span class="ver strong">{{item.expressFullName}}</span><span class="ver colorc0 ml10">(Arrival time:{{item.expressDate}})({{item.totalWeight}}Kg)</span></label>'+
                          ' <span class="pull-right color0"><span ng-if="item.shippingDiscount>0" class="colord0 lineThrough">{{item.shippingMoney|moneyChange}}</span><span ng-if="item.shippingDiscount>0" class="red plr10">({{item.shippingDiscount|moneyChange}} off)</span>{{(item.shippingMoney-item.shippingDiscount)|moneyChange}}</span>'+
                          ' <p class="colorc0 ml15 mb0 font12">{{item.expressRemark}}</p>'+
                          ' <p ng-if="item.expressRemark2" class="red ml15 mb0 font12">{{item.expressRemark2}}</p>'+
                          ' </div>'+
                      '  </div>',*/
            link:function(scope,element){
            }
        }
    });
    /*总价*/
    directiveModule.directive('summary', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {
                orderMoney:"=",//金额
                shippingMoney:'='//配送金额
            },
            transclude : true,
            template: '   <div>'+
            <!--hr-->
            ' <hr class="borderColorc mt5">'+
                '<h2 class="nowrap line40 font16 strong">SUMMARY</h2>'+
                '<p class="color8 font16 line30">Merchandise Total<span class="pull-right color0">{{orderMoney|moneyChange}}</span></p>'+
                ' <p class="color8 font16 line30">Shipping<span class="pull-right color0">{{shippingMoney|moneyChange}}</span></p>'+
        <!--hr-->
            '<hr class="borderColorc mlr10">'+
        ' <p class="orange font16 ml15 mt5">Grand Total:<span class="pull-right ">{{(orderMoney+shippingMoney)|moneyChange}}</span></p>'+
        '<div class="text-center mt25 mb30" ng-transclude></div>'+
        <!--hr-->
        '<hr class="borderColorc">'+
        '</div>',
            link:function(scope,element){
            }
        }
    });
    /*支付方式*/
    directiveModule.directive('paymentMethod', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {
                formModel:"="//表单
            },
            transclude : true,
            template: '  <div class="pr20 pl12">'+
                ' <p class="red">*SSL encrypted payment</p>'+
                '  <div class="line30 mb20 clearfix">'+
                '  <label class="mb0"><input ng-model="formModel.paymentMethod" value="cart" class="ver mt0" type="radio" name="payment"> <span class="ver">Credit Cart / Debit Card</span></label>'+
                ' <div class="pull-right"><span class="icon-bg icon-bank1 ml20"></span><span class="icon-bg icon-bank2 ml20"></span><span class="icon-bg icon-bank3 ml20"></span></div>'+
                ' </div>'+
                ' <div class="pl25" ng-transclude></div>'+
                ' <div class="line30 mb20 clearfix mt15">'+
                ' <label class="mb0"><input ng-model="formModel.paymentMethod" value="paypal" class="ver mt0" type="radio" name="payment"> <span class="ver">PayPal</span></label>'+
                '  <div class="pull-right"><span class="icon-bg icon-bank4"></span></div>'+
                '  </div>'+
                '  </div>',
            link:function(scope,element){
            }
        }
    });
});
