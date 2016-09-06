const electron = require( 'electron' )
const app = electron.app
const BrowserWindow = electron.BrowserWindow

app.on( 'ready', function(){
	main_window = new BrowserWindow( {
		width: 400,
		height:300
	});

	main_window.loadURL( 'file://'+__dirname+'/app.html' )
	main_window.on( 'closed', function(){
		main_window = null
	});
})

