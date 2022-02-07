var res_temp = "";
var res_humi = "";

if (context.global.localHumid() >= context.global.humOpen()){
  res_humi = "mais úmido";
}else{
  res_humi = "mais seco";
}

if (context.global.localTemp() >= context.global.tempOpen()){
  res_temp = "mais quente";
}else{
  res_temp = "mais frio";
}

if(context.global.locate() == "fpolis"){
    context.global.locate = function(){return "Florianopolis"};
}

var res = {payload: " O clima Local está " + 
res_temp +" e também " + res_humi + " em relacao à cidade "+
context.global.locate()};

return res;
