#include <windows.h>

#include <ogl.h>
#include <render_api.h>
#include <debug.h>

char *COpenGLRend::get_extensions(void)
{
	return (char *) glGetString(GL_EXTENSIONS);
}

int COpenGLRend::query_extension(char *extName)
{
    char *p;
    char *end;
    int extNameLen;   

    extNameLen = strlen(extName);
        
    p = (char *)glGetString(GL_EXTENSIONS);
    if (NULL == p) {
        return 0;
    }

    end = p + strlen(p);   

    while (p < end) {
        int n = strcspn(p, " ");
        if ((extNameLen == n) && (strncmp(extName, p, n) == 0)) {
            return 1;
        }
        p += (n + 1);
    }
    return 0;
}

int COpenGLRend::init_extensions(void)
{
/*
	if(!glh_init_extensions("GL_NV_vertex_program " "GL_NV_register_combiners ")) 
		return 0;
	else
		return 1;

  */
	return 1;
}

