/*!
 * ASP.NET SignalR JavaScript Library v1.1.1
 * http://signalr.net/
 *
 * Copyright Microsoft Open Technologies, Inc. All rights reserved.
 * Licensed under the Apache 2.0
 * https://github.com/SignalR/SignalR/blob/master/LICENSE.md
 *
 */

//<reference path="jquery-1.9.1.js" />
//<reference path="jquery.signalR.js" />
//<reference path="config.js" />
(function ($, window) {
    /// <param name="$" type="jQuery" />
    "use strict";

    if (typeof ($.signalR) !== "function") {
        throw new Error("SignalR: SignalR is not loaded. Please ensure jquery.signalR-x.js is referenced before ~/signalr/hubs.");
    }

    var signalR = $.signalR;

    function makeProxyCallback(hub, callback) {
        return function () {
            // Call the client hub method
            callback.apply(hub, $.makeArray(arguments));
        };
    }

    function registerHubProxies(instance, shouldSubscribe) {
        var key, hub, memberKey, memberValue, subscriptionMethod;

        for (key in instance) {
            if (instance.hasOwnProperty(key)) {
                hub = instance[key];

                if (!(hub.hubName)) {
                    // Not a client hub
                    continue;
                }

                if (shouldSubscribe) {
                    // We want to subscribe to the hub events
                    subscriptionMethod = hub.on;
                }
                else {
                    // We want to unsubscribe from the hub events
                    subscriptionMethod = hub.off;
                }

                // Loop through all members on the hub and find client hub functions to subscribe/unsubscribe
                for (memberKey in hub.client) {
                    if (hub.client.hasOwnProperty(memberKey)) {
                        memberValue = hub.client[memberKey];

                        if (!$.isFunction(memberValue)) {
                            // Not a client hub function
                            continue;
                        }

                        subscriptionMethod.call(hub, memberKey, makeProxyCallback(hub, memberValue));
                    }
                }
            }
        }
    }

    $.hubConnection.prototype.createHubProxies = function () {
        var proxies = {};
        this.starting(function () {
            // Register the hub proxies as subscribed
            // (instance, shouldSubscribe)
            registerHubProxies(proxies, true);

            this._registerSubscribedHubs();
        }).disconnected(function () {
            // Unsubscribe all hub proxies when we "disconnect".  This is to ensure that we do not re-add functional call backs.
            // (instance, shouldSubscribe)
            registerHubProxies(proxies, false);
        });

        proxies.thermostat = this.createHubProxy('thermostat-v1'); 
        proxies.thermostat.client = { };
        proxies.thermostat.server = {
            changeSetting: function (icdId, feature, value) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["ChangeSetting"], $.makeArray(arguments)));
            },

            deleteSchedule: function (icdId, objectId) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["DeleteSchedule"], $.makeArray(arguments)));
            },

            saveSchedule: function (icdId, newSchedule) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SaveSchedule"], $.makeArray(arguments)));
            },

            setAutoCool: function (icdId, targetTemp, units) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetAutoCool"], $.makeArray(arguments)));
            },

            setAutoHeat: function (icdId, targetTemp, units) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetAutoHeat"], $.makeArray(arguments)));
            },

            setCool: function (icdId, targetTemp, units) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetCool"], $.makeArray(arguments)));
            },

            setFanMode: function (icdId, fanMode) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetFanMode"], $.makeArray(arguments)));
            },

            setHeat: function (icdId, targetTemp, units) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetHeat"], $.makeArray(arguments)));
            },

            setHoldMode: function (icdId, holdMode) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetHoldMode"], $.makeArray(arguments)));
            },

            setScheduleActive: function (icdId, objectId) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetScheduleActive"], $.makeArray(arguments)));
            },

            setScheduleMode: function (icdId, scheduleMode) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetScheduleMode"], $.makeArray(arguments)));
            },

            setSystemMode: function (icdId, systemMode) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["SetSystemMode"], $.makeArray(arguments)));
            },

            subscribe: function (icdId) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["Subscribe"], $.makeArray(arguments)));
            },

            unsubscribe: function (icdId) {
                return proxies.thermostat.invoke.apply(proxies.thermostat, $.merge(["Unsubscribe"], $.makeArray(arguments)));
            }
        };

        return proxies;
    };

    signalR.hub = $.hubConnection(Sensi.Config.Realtime, { useDefaultPath: false });
    $.extend(signalR, signalR.hub.createHubProxies());

}(window.jQuery, window));