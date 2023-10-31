docker run -d \
      	--name=kicks \
      	-e HUSER=docker \
      	-e HPASS=docker \
      	-p 2121:21 \
      	-p 2323:23 \
      	-p 3270:3270 \
      	-p 3505:3505 \
      	-p 3506:3506 \
      	-p 8888:8888 \
        -v $(pwd)/kicks:/config \
        -v $(pwd)/kicks/printers:/printers \
        -v $(pwd)/kicks/punchcards:/punchcards \
        -v $(pwd)/kicks/logs:/logs \
        -v $(pwd)/kicks/dasd:/dasd \
        -v $(pwd)/kicks/certs:/certs \
      	--restart unless-stopped \
      	mainframed767/kicks:1.5.1

