import flask

app = flask.Flask(__name__)

@app.route('/')
def index():
	return flask.redirect('http://localhost/server.php')

if __name__=="__main__":
	app.run(host='192.168.56.1', debug=True)
