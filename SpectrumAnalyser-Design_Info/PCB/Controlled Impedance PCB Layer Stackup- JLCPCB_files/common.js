/**
 * Created by win7 on 2017/10/12.
 */
/**
 * 公共函数
 */
/*图片加载错误*/
function nofind(obj){
    if(obj["name"]==3){
        /*钢网*/
        obj.src = '../client/image/cart_steel.58c85990.png';
    }else{
        /*pcb*/
        obj.src = '../client/image/noImg.9f6758f0.png';
    }
    obj.onerror = null;
    obj.parentNode.setAttribute("href","javascript:void(0);");
}
//读取cookies
function getCookie(name) {
    var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
    if(arr=document.cookie.match(reg))
        return unescape(arr[2]);
    else
        return null;
}
//设置cookies
function setCookie(name,value,time) {
    var strsec = getsec(time);
    var exp = new Date();
    exp.setTime(exp.getTime() + strsec*1);
    document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString()+"; path=/";
}
function getsec(str) {
    var str1=str.substring(1,str.length)*1;
    var str2=str.substring(0,1);
    if (str2=="s") {
        return str1*1000;
    } else if (str2=="h") {
        return str1*60*60*1000;
    } else if (str2=="d") {
        return str1*24*60*60*1000;
    }
}
define(["jquery","bootstrap"],function ($) {
    /**
     * 公用弹出窗口
     * @param url
     * @param title
     * @param width
     * @param minHeight
     * @param resize 是否允许最大化
     * @param aotuscrollObj
     */
    /*提示框*/
    $('[data-toggle="tooltip"]').tooltip();

    $(function(){
        $(".modal").on("hidden.bs.modal",function(){
            $("body").removeAttr("style");
        });
    });
    /*显示弹框*/
    var showModal=function showModal(id){
        $("#"+id).modal("show").on("shown.bs.modal",function(){});
    };
    /*隐藏弹框*/
    var hideModal=function hideModal(id){
        $(id).modal("hide").on("shown.bs.modal",function(){});
    };
    /*确定弹框*/
    var alertModal=function alertModal(msg){
        $("#msg").html(msg);
        $("#alert").modal("show").on("shown.bs.modal",function(){});
    };
    /*提示弹框*/
    var noticeModal=function noticeModal(msg){
        $("#notice").html(msg);
        $("#noticeModal").modal("show").on("shown.bs.modal",function(){});
    };
    /*加载弹框*/
    var loadingModal=function loadingModal(msg){
        if(msg){
            $("#loading").html(msg);
        }
        $("#loadingModal").modal("show");
    };
    var closeLoadingModal=function closeLoadingModal(){
        $("#loadingModal").modal("hide");
    };
    /*倒计时*/
    var countDown= function countDown(date,hour,minute,second,callback){
        var intervalTime=window.setInterval(function(){
            if(parseInt(second)>0){
                second--;
                if(second<10){
                    second="0"+second;
                }
            }else{
                if(parseInt(minute)>0){
                    minute--;
                    second=59;
                    if(minute<10){
                        minute="0"+minute;
                    }
                }else{
                    if(parseInt(hour)>0){
                        hour--;
                        minute=59;
                        if(hour<10){
                            hour="0"+hour;
                        }
                    }else{
                        if(parseInt(date)>0){
                            date--;
                            hour=23;
                            if(date<10){
                                date="0"+date;
                            }
                        }else{
                            window.clearInterval(intervalTime);
                        }
                    }
                }
            }
            if(typeof callback == "function")
                callback(date,hour,minute,second);
        },1000);
    };
    return {
        showModal:showModal,
        hideModal:hideModal,
        alertModal: alertModal,
        noticeModal: noticeModal,
        closeLoadingModal: closeLoadingModal,
        loadingModal: loadingModal,
        countDown: countDown
    };

});