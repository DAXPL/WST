default:
	echo "No default command. Check Makefile"

setup:
	docker compose build
	
train:
	docker compose up

enter:
	docker compose run --rm ml_model bash

clean:
	docker compose run --rm ml_model bash -c "rm -rf /work/results/*"