var Sensi = Sensi || {};		

// SignalR
Sensi.Hub = $.connection.hub;         
Sensi.Thermostat = $.connection.thermostat;

Sensi.UserName = "andrew.kutta@emerson.com";
Sensi.Password = "Emerson1";
Sensi.ICDID = "36-6f-92-ff-fe-00-0a-59";

// End Points
Sensi.AuthorizeEndpoint = Sensi.Config.Api + 'authorize';
Sensi.ThermostatsEndpoint = Sensi.Config.Api + 'thermostats';

$(function () {
	$.support.cors = true;
	$.ajaxSetup( {
		xhrFields: { withCredentials: true }
		});

	// Login starts the SignalR Hub as well
	Sensi.LoginAndStartSignalR();
	
});

Sensi.LoginAndStartSignalR = function() {
	Sensi.Login(Sensi.UserName,Sensi.Password).done(function() {			
					
			Sensi.StartSignalR().done(function() {
				Sensi.log("Subscribing to " + Sensi.ICDID);
				Sensi.Thermostat.server.subscribe(Sensi.ICDID);
			});
			
			/*
			Can get all of the ICDID's associated with this account if you uncomment below call
			
			Sensi.log("Retreiving Thermostats");
			Sensi.GetThermostatList().done(function (data) {
				Sensi.log(data); 
			});*/
		});
}

Sensi.ConnectionError = function() {
	Sensi.log("Connection Error....");
	Sensi.Hub.stop();
	Sensi.LoginAndStartSignalR();
};
Sensi.Disconnected = function() {
	Sensi.log('Disconnected');
};

Sensi.StartSignalR = function() {

	// Declare a proxy to reference the hub.    			
	Sensi.Hub.error(Sensi.ConnectionError);
	Sensi.Hub.disconnected(Sensi.Disconnected);

	Sensi.Thermostat.client.online = function(icd,model) {
		Sensi.log("Online");
	}
	
	Sensi.Thermostat.client.update = function (icd, model) {
		Sensi.log("Update");
		Sensi.log(model);
	}
	
	Sensi.Thermostat.client.offline = function (icd) {
		Sensi.log("Offline");
	}
	
	Sensi.Thermostat.client.error = function (icd, model) {
		Sensi.log("Error");
	}
	
	return Sensi.Hub.start();
}

Sensi.Login = function (username, password) {
	return $.ajax({
		type: "POST",
		cache: false,
		contentType: 'application/json',
		headers: { "Content-Type": "application/json", "Accept": "application/json; version=1", "X-Requested-With": "XMLHttpRequest" },
		dataType: 'json',
		url: Sensi.AuthorizeEndpoint,
		data: JSON.stringify({ UserName: username, Password: password }),
		success: function(response) {
			Sensi.Auth = response;
		}
	})
	.fail(function(ajax, error, errorMessage) {
		Sensi.log(errorMessage);
	})
	.success(function(ajax, response) {
		Sensi.log(response);
	});
};

Sensi.GetThermostatList = function() {
	Sensi.log("Retreiving Thermostats");
	return Sensi.JsonRequest("GET", Sensi.ThermostatsEndpoint)
		.fail(function (ajax, error, errorMessage) { Sensi.log(errorMessage); });
};

Sensi.JsonRequest = function(method, url, payload) {
	var data = payload ? JSON.stringify(payload) : undefined;
	var request = Sensi.flXHR ? url + '?Accept=' + Sensi.Accept : url;

	return $.ajax({
		url: request,
		data: data,
		headers: { "Content-Type": "application/json", "Accept": "application/json; version=1", "X-Requested-With": "XMLHttpRequest" },
		type: method,
		contentType: 'application/json',
		dataType: 'json'
	});
};

Sensi.log = function(data) {
	window.console && window.console.log(data);
};
