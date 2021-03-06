 $(document).ready(function () {
      
	  // Setting up connection to Azure Blob Storage
	  const account = {
		name: "memgstorage",
		sas: "?sv=2017-11-09&ss=bfqt&srt=sco&sp=rwdlacup&se=2019-10-10T11:45:52Z&st=2018-10-10T03:45:52Z&spr=https,http&sig=z462gNN6P5Ty7Rby8o%2Bfr6LRoV1USIHC2Y%2FT%2FnJ7NyI%3D"
		};
	  const blobUri = 'https://' + account.name + '.blob.core.windows.net';
	  const blobService = AzureStorage.Blob.createBlobServiceWithSas(blobUri, account.sas);
	  var randID = 0;
	  // Reading the names of the blobs into the list
	  blobService.listBlobsSegmented("memgcontainer", null, (error, results) => {
		  if(error){
			  
		  } else{
			  results.entries.forEach(blob => {
          randID++;
		  console.log(blob.name.slice(24,26));
		  if(parseInt(blob.name.slice(24,26))>=14){
			var blobName = blob.name.slice(13,21) + (parseInt(blob.name.slice(21,23))+1) + " - " + (parseInt(blob.name.slice(24,26))-14) + ":" + blob.name.slice(27,29);
		  }
		  else{
			var blobName = blob.name.slice(13,23) + " - " + (parseInt(blob.name.slice(24,26))+10) + ":" + blob.name.slice(27,29);
		  }
          var ele = "<tr><th>" +randID+"</th><td>"+blobName+"</td><td>"+( Math.round(Math.random()*2 + 1) )+"</td><td style=\"text-align: center;\"><button onclick=\"refreshGraph2('"+blob.name+"')\" class=\"btn btn-warning\">Show</button></td></tr>";
          
          $("#sessionsBody").prepend(ele);
          
			  });
		    }
	  });
	  
	  // Setting up an empty graph

	g = new Dygraph(
		
	document.getElementById("div_g"), "X\n",
	{
		drawPoints:true,
		labels: ['Sample', 'Value'],
		legend: 'always',
	});

  var data = [];
  var startVal = 2;
  for (var i = 1; i <= 30; i++) {
    var day = i % 28;
    if (day < 10) { day = "0"+i; }
    
    if (startVal < 0) { startVal = 0; }
    
    var curVal = [new Date("2018-04-"+day), startVal];
    startVal = startVal + Math.round(Math.random()*3 - 1);
    data.push(curVal);
  }

  var g = new Dygraph(document.getElementById("div_Progress"), data,
                      {
                        ylabel: "Number of Reps",
                        drawPoints: true,
                        labels: ['Time', 'Number of Reps']
                      });
});

function refreshGraph2(blobname) {

	  // Setting up connection to Azure Blob Storage
	  const account = {
		name: "memgstorage",
		sas: "?sv=2017-11-09&ss=bfqt&srt=sco&sp=rwdlacup&se=2019-10-10T11:45:52Z&st=2018-10-10T03:45:52Z&spr=https,http&sig=z462gNN6P5Ty7Rby8o%2Bfr6LRoV1USIHC2Y%2FT%2FnJ7NyI%3D"
		};
	  const blobUri = 'https://' + account.name + '.blob.core.windows.net';
	  const blobService = AzureStorage.Blob.createBlobServiceWithSas(blobUri, account.sas);
	
	  // Reading Data from the Blob
	  blobService.getBlobToText("memgcontainer", blobname, (error, results) =>{
			if(error){
			  
			}
			else{
				var datastring = results;
				var pos = datastring.indexOf("EMG");
				var datas = [];
				while (pos > 0){
				var res = datastring.slice(pos+7, pos+607);
				datas.push(res);
				pos = datastring.indexOf("EMG",pos+10);
				}
				
				var decodedDatas = [];
				var i,j;
				
				for(i=0;i < datas.length; i++){
					decodedDatas.push(decode(datas[i]));
				}
				
				var samples = [];
				
				for(i=0;i < decodedDatas.length; i++){
					for(j = 0; j < decodedDatas[i].length; j++){
						samples.push(decodedDatas[i][j]);
						//graphData.push([(300*i)+j, decodedDatas[i][j]]);
					}
				}
				
				var smoothSamples = [];
				for(i=0;i < samples.length; i++){
					if(i==0){
						smoothSamples.push(0.1*samples[i]);
					}
					else{
						smoothSamples.push(0.98*smoothSamples[i-1]+0.1*samples[i]);
					}
				}
				
				var graphData = [];
				for(i=0;i < samples.length; i++){
					if(i%25==0)
					graphData.push([i, samples[i], smoothSamples[i], 600]);
					else
					graphData.push([i, samples[i], smoothSamples[i], null]);
				}
	
				g = new Dygraph(
		
				document.getElementById("div_g"), graphData,
				{
          ylabel: "Muscle Activation",
					title: "Tim's Wicked Quad",
					legend: "always",
					labels: ['Sample', 'Raw', 'Smooth', 'Threshold'],
					series: {
						'Smooth':{
							strokeWidth: 3,
							color: "#A93226"
						},
						
						'Raw':{
							color: "#85929E"
						},
						
						'Threshold':{
							strokeWidth: 0,
							drawPoints: true,
							pointSize: 1,
							highlightCircleSize: 0
						}
					}

				});
				

			}
			

		});
	
};




function decode(dataStr) {
    var encodedData = [];
    var n = 0;
    for(var i = 0; i < dataStr.length; ++i) {
      encodedData[n] = dataStr[i] + dataStr[i+1];
      ++n;
      ++i;
    }
      
    validChars = [
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 
        'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B',
        'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '#',  
        '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<',  
        '=', '>', '?', '[', ']', '^', '_', '{', '|', '}', '~'];
    
    var decodedData = [encodedData.length];
    
    for(var n = 0; n < encodedData.length; ++n) {
        
      var result = encodedData[n];
      var encodedVal = 0;
        
      for(var i = 0; i < result.length ; ++i) {
        encodedVal = encodedVal * validChars.length + validChars.indexOf( result[i] );
      }
        
      decodedData[n] = encodedVal;
    }
    return decodedData;
};

function toggleCompare(){
	var x = document.getElementById("compareTable");
	if(x.style.display === "none"){
		x.style.display = "block";
	} else{
		x.style.display = "none";
	}
}
	  

