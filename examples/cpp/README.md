# Readme

Start Zipking server
-------------------

curl -sSL https://zipkin.io/quickstart.sh | bash -s

java -jar zipkin.jar

> examples/bin/example --transport=http --service=commandline_service

Run with Scribe
---------------

SCRIBE_ENABLED=true java -jar zipkin.jar

> examples/bin/example --transport=scribe --service=commandline_service
