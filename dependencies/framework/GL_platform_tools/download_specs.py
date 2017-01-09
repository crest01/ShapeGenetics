


import urllib.request


def main():
	response = urllib.request.urlopen("https://cvs.khronos.org/svn/repos/ogl/trunk/doc/registry/public/api/gl.xml")
	data = response.read()
	with open("gl.xml", "wb") as file:
		file.write(data)


if __name__ == "__main__":
	main()
