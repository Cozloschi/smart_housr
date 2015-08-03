var ml = require('machine_learning');
var http = require('http');
var url = require('url');
var make_req = require("request");
 
var array_habbits = [];
var array_data = []; //possible habbits
var array_results = [];
var get_param = '';

//Lets define a port we want to listen to
const PORT=8080; 

function getKey(data) {
  for (var prop in data)
    return prop;
}

	function get_length(list){
		k=0;
		for(var i in list)
		 if( typeof list[i] !== 'undefined')
			k++;
		
		return k;
	}
	
	function check_habbit(list){
		
		var join_list = list.join(',');
		//console.log(get_length(array_habbits))
		for(var i =0;i<=get_length(array_habbits);i++){
		if( typeof array_habbits[i] != 'undefined'){
		  if(String(join_list) === String(array_habbits[i].join(',')+','+array_results[i])){
			  return true;
		  }		
		}
		}
		return false;
	}
		
function handleRequest(request, response){
     params = url.parse(request.url,true);
	 get_param += params.query.data;
	 if(get_param.length <= 1 || (get_param != '3' || get_param != '2'))  response.end('<html><head><link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon"></head><body>1</body></html>');

	 
if(get_param.length == 3){
	var array = [get_param[0],get_param[1],get_param[2]];   
	
	get_param = '';
	
	if(check_habbit(array))
	{
		console.log('Habbit already exists');
		response.end('<html><head><link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon"></head><body>1</body></html>');
	    return;
	}
	if(get_length(array_data) > 0){
	console.log('habbit added');
	for(var i = 0;i<=get_length(array_data);i++){
		if(typeof array_data[i] !== 'undefined')
		{
		   if(array_data[i].join(',') == array.join(','))
           {
			   array_data.splice(i-1, i+1);				   
			   array_habbits.push([array[0],array[1]]);
			   array_results.push(array[2]);
		   }else{
			   array_data.push(array);
		   }			   
		  	
		}
	}
} 
else
 array_data.push(array);	
   get_param = '';
   //console.log(array_habbits);   // console.log(array_results);   
   console.log('pushed to data array');
   console.log(array_data);
   response.end('<html><head><link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon"></head><body>It Works!! Path Hit: ' + request.url+ "</body></html>");
}
else{
if(get_param.length == 2)
{
if(array_habbits.length == 0){
	
response.end('<html><head><link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon"></head><body>1</body></html>');

	return;
}
if(get_length(array_results) == 1){
	array_results.push('x');
	array_results.push('x');

	array_habbits.push(['x','x']);
	array_habbits.push(['x','x']);
}



var d_h = array_habbits;
var d_r  = array_results;

var dt = new ml.DecisionTree({
    data : d_h,
    result : d_r
});
 
dt.build();
 
dt.print();
 
var classify =  dt.classify([get_param[0],get_param[1]]);

 
 // debugger
 for(var i =0;i<=get_length(array_habbits);i++)
	 if( typeof array_habbits[i] !== 'undefined')
	 array_habbits[i] = array_habbits[i].splice(0,2);
 
var key = getKey(classify);
 

make_req("192.168.1.177?"+key, function(error, response, body) {
  console.log(body);
});
	

response.end('<html><head><link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon"></head><body>'+getKey(classify)+'</body></html>');

}
}
console.log(array_habbits);
}
//Create a server
var server = http.createServer(handleRequest);

//Lets start our server
server.listen(PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", PORT);
});
