from flask import Flask, render_template, jsonify, request

import paho.mqtt.client as paho

app = Flask(__name__)

import ssl

# MQTT configurationc
mqtt_broker = "project1-l7r2b7.a01.euc1.aws.hivemq.cloud"
mqtt_port = 8883
mqtt_topic = "status"
mqtt_username = "admin"
mqtt_password = "Admin123"

latest_message = "You Are Connected"

# Callback when a new MQTT message is received
def on_message(client, userdata, message):
    global latest_message
    latest_message = message.payload.decode('utf-8')

# Use the latest callback API version

mqtt_client = paho.Client()

mqtt_client.tls_set(tls_version=ssl.PROTOCOL_TLS)
mqtt_client.username_pw_set(username=mqtt_username, password=mqtt_password)
mqtt_client.on_message = on_message




mqtt_client.connect(mqtt_broker, mqtt_port)
mqtt_client.subscribe(mqtt_topic)
mqtt_client.loop_start()
mqtt_client.subscribe(mqtt_topic)
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_status', methods=['GET'])
def get_status():
    return jsonify(status=latest_message)

@app.route('/publish_message', methods=['POST'])
def publish_message():
    topic = request.form['topic']
    message = request.form['message']
    
    try:
        mqtt_client.publish(topic, message)
        return jsonify(status='success', message='Message published')
    except Exception as e:
        return jsonify(status='error', message=str(e))


if __name__ == "__main__":
    app.run(host='0.0.0.0')
  ##Abdelhamid97.pythonanywhere.com.
