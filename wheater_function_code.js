var local = {payload: msg.payload.location}
var temp = {payload: msg.payload.tempc}
var humid = {payload: msg.payload.humidity}
context.global.locate = function(){return local.payload};
context.global.tempOpen = function(){return temp.payload};
context.global.humOpen = function(){return humid.payload};
return [local, temp, humid];