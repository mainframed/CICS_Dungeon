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
        -v /home/defcon31/DEV/CICSDGN/kicks:/config \
        -v /home/defcon31/DEV/CICSDGN/kicks/printers:/printers \
        -v /home/defcon31/DEV/CICSDGN/kicks/punchcards:/punchcards \
        -v /home/defcon31/DEV/CICSDGN/kicks/logs:/logs \
        -v /home/defcon31/DEV/CICSDGN/kicks/dasd:/dasd \
        -v /home/defcon31/DEV/CICSDGN/kicks/certs:/certs \
      	--restart unless-stopped \
      	mainframed767/kicks:1.5.0

