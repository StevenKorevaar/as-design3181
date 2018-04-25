 $(document).ready(function () {
      
	  // Setting up connection to Azure Blod Storage
	  const account = {
		name: "laikamemgstorage",
		sas: "sv=2017-04-17&ss=bfqt&srt=sco&sp=rwdl&st=2018-04-21T05%3A21%3A21Z&se=2019-04-22T05%3A21%3A00Z&sig=lXzkeEHwkI1KF5TKHvAeSD3x7XABYERYs7%2Fj8VIRxEE%3D"
		};
	  const blobUri = 'https://' + account.name + '.blob.core.windows.net';
	  const blobService = AzureStorage.Blob.createBlobServiceWithSas(blobUri, account.sas);
	  
	  // Reading Data from the Blob
	  blobService.getBlobToText("laika-memg-container","laika-hub/00/2018/04/22/10/12", (error, results) =>{
			if(error){
			  
			}
			else{
				var datastring = results;
				var pos = datastring.indexOf("TRIANGLE");
				var datas = [];
				while (pos > 0){
				var res = datastring.slice(pos+12, pos+615);
				datas.push(res);
				pos = datastring.indexOf("TRIANGLE",pos+10);
				}
				var i,j;
				var numberOfSamples = (datas[0].length)/3;
				var samples = [];
				for(i = 0; i < datas.length; i++){
					for( j = 0; j < numberOfSamples; j++){
					var sampleHex = datas[i].slice(j*3,j*3+3);
					samples.push(parseInt(sampleHex,16));
					}
				}
			}
			
			var graphData = [];
			for (i = 0; i < samples.length; i++){
			graphData.push([i, samples[i]]);
			}
					
			g = new Dygraph(
		
			document.getElementById("div_g"),graphData,
			{
				drawPoints:true,
				showRoller:true,
				labels: ['Sample', 'Value']
					
			});
			
		});
	  
}
	  
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
);