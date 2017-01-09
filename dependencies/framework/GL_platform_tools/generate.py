


import lxml.etree


parser = lxml.etree.XMLParser()
parser.resolvers.add(lxml.etree.Resolver())

def loadDatabase():
	with open("gl.xml", "rb") as file:
		return lxml.etree.parse(file, parser)

def loadStylesheet(filename):
	with open(filename, "rb") as file:
		root = lxml.etree.parse(file, parser)
		return lxml.etree.XSLT(root)

def writeResult(result, filename):
	with open(filename, "wb") as file:
		file.write(result)

def genEGL(database_root):
	gl_h = loadStylesheet("gl.h.xslt")
	context_h = loadStylesheet("egl_glcore_context.h.xslt")
	context_cpp = loadStylesheet("egl_glcore_context.cpp.xslt")
	
	writeResult(gl_h(database_root), "source/egl/include/GL/gl.h")
	writeResult(context_h(database_root), "source/egl/glcore/context.h")
	writeResult(context_cpp(database_root), "source/egl/glcore/context.cpp")

def genWin32(database_root):
	gl_h = loadStylesheet("win32_gl.h.xslt")
	context_h = loadStylesheet("win32_glcore_context.h.xslt")
	context_cpp = loadStylesheet("win32_glcore_context.cpp.xslt")
	
	writeResult(gl_h(database_root), "source/win32/include/GL/gl.h")
	writeResult(context_h(database_root), "source/win32/glcore/context.h")
	writeResult(context_cpp(database_root), "source/win32/glcore/context.cpp")

def genX11(database_root):
	gl_h = loadStylesheet("gl.h.xslt")
	
	writeResult(gl_h(database_root), "source/x11/include/GL/gl.h")

def main():
	database_root = loadDatabase()
	
	genEGL(database_root)
	genWin32(database_root)
	genX11(database_root)


if __name__ == "__main__":
	main()
