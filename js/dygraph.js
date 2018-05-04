 $(document).ready(function () {
      
	  // Setting up connection to Azure Blod Storage
	  const account = {
		name: "laikamemgstorage",
		sas: "sv=2017-04-17&ss=bfqt&srt=sco&sp=rwdl&st=2018-04-21T05%3A21%3A21Z&se=2019-04-22T05%3A21%3A00Z&sig=lXzkeEHwkI1KF5TKHvAeSD3x7XABYERYs7%2Fj8VIRxEE%3D"
		};
	  const blobUri = 'https://' + account.name + '.blob.core.windows.net';
	  const blobService = AzureStorage.Blob.createBlobServiceWithSas(blobUri, account.sas);
	  
	  // Reading the names of the blobs into the list
	  blobService.listBlobsSegmented("laika-memg-container", null, (error, results) => {
		  if(error){
			  
		  } else{
			  results.entries.forEach(blob => {
				  var x = document.getElementById("mySelect");
				  var option = document.createElement("option");
				  option.text = blob.name;
				  x.add(option)
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
  /*
  // It sucks that these things aren't objects, and we need to store state in window.
  window.intervalId = setInterval(function() {
    var x = new Date();  // current time
    var y = Math.random();
    data.push([x, y]);
    g.updateOptions( { 'file': data } );
  }, 1000);
  */
    
    
});

function refreshGraph() {
	
	  // Setting up connection to Azure Blob Storage
	  const account = {
		name: "laikamemgstorage",
		sas: "sv=2017-04-17&ss=bfqt&srt=sco&sp=rwdl&st=2018-04-21T05%3A21%3A21Z&se=2019-04-22T05%3A21%3A00Z&sig=lXzkeEHwkI1KF5TKHvAeSD3x7XABYERYs7%2Fj8VIRxEE%3D"
		};
	  const blobUri = 'https://' + account.name + '.blob.core.windows.net';
	  const blobService = AzureStorage.Blob.createBlobServiceWithSas(blobUri, account.sas);
	
	  // Reading Data from the Blob
	  blobService.getBlobToText("laika-memg-container", mySelect.options[mySelect.selectedIndex].value, (error, results) =>{
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
					if(i%1==0)
					graphData.push([i, samples[i], smoothSamples[i], 300]);
					else
					graphData.push([i, samples[i], smoothSamples[i], null]);
				}
				/*
				for(i=0;i < graphData.length; i++){
					console.log(graphData[i][0]);
				}
        */
	
				g = new Dygraph(
		
				document.getElementById("div_g"), graphData,
				{
					title: "Tim's Wicked Quad",
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
				
					//drawPoints:true,
					//showRoller:true,
					//labels: ['Sample', 'Value']
						
				});
				
				/*
				var i,j;
				var numberOfSamples;
				var samples = [];
				for(i = 0; i < datas.length; i++){
					numberOfSamples = datas[i].length/3;
					console.log(numberOfSamples);
					for( j = 0; j < numberOfSamples; j++){
					var sampleHex = datas[i].slice(j*3,j*3+3);
					samples.push(parseInt(sampleHex,16));
					}
				}
				*/
			}
			
			/*
			console.log(datas.length);
			
			var graphData = [];
			for (i = 0; i < samples.length; i++){
			graphData.push([i, samples[i]]);
			}
			
			console.log(samples.length);
					
			g = new Dygraph(
		
			document.getElementById("div_g"), graphData,
			{
				drawPoints:true,
				showRoller:true,
				labels: ['Sample', 'Value']
					
			});
			*/
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
	  
	  /*
	  var data = [];
      var t = new Date();
      for (var i = 100; i >= 0; i--) {
        var x = new Date(t.getTime() - i * 1000);
        data.push([x, Math.random()]);
      }

      var g = new Dygraph(document.getElementById("div_g"), data,
                          {
                            drawPoints: true,
                            showRoller: true,
                            valueRange: [0.0, 1.2],
                            labels: ['Time', 'Random']
                          });
      // It sucks that these things aren't objects, and we need to store state in window.
      window.intervalId = setInterval(function() {
        var x = new Date();  // current time
        var y = Math.random();
        data.push([x, y]);
        data.shift();
          
        g.updateOptions( { 'file': data } );
      }, 1000);
    }
	*/
