build:
	gcc tema1.c LinkedList.c -g -o tema
	

clean:
	rm tema
	

checker:
	python3 checker.py
