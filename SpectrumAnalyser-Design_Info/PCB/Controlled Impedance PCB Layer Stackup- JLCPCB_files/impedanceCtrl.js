/**
 * Created by win7 on 2017/10/12.
 */
define(['../../../../client/js/app.9566a3b7.js?ver=2', '../../../../client/js/common.e91a632e.js'],
    function (app, common) {
        app.controller('impedanceCtrl',
            function ($scope, $rootScope, httpService, httpApi, $location, $timeout, myServer) {
                //$(".minHead,.commonFooter").hide();
                $scope.rootUrl = httpApi.rootUrl;//根域名
                $scope.form = {
                    layer: 4,//板层
                    thickness: 1.6,//板厚
                    thickness1: 1.6,//板厚

                };
            }
        );
    });