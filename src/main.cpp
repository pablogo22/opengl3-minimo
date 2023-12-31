// Ejemplo mínimo de código OpenGL, usa OpenGL 3.3 + GLSL 3.3 + GLFW



// includes de la librería estándard de C++
#include <cassert>   // 'assert' (enforce preconditions)
#include <cstring>   // 'strlen' (to compile shaders)
#include <iostream>  // 'cout' and such
#include <iomanip>   // set precision and such
#include <vector>    // 'std::vector' types

// incluir cabeceras de OpenGL y GLM
#include "glincludes.h"

// incluir cabeceras auxiliares para shaders, vaos y vbos.
#include "cauce.h"      // clase 'Cauce'
#include "vaos-vbos.h"  // clases 'DescrVAO', 'DescrVBOAtribs' y 'DescrVBOInds' 

// ---------------------------------------------------------------------------------------------
// Constantes y variables globales

// constexpr GLuint
//     ind_atrib_posiciones = 0,      // índice del atributo de vértice con su posiciones (debe ser el índice 0, siempre)
//     ind_atrib_colors    = 1,      // índice del atributo de vértice con su color RGB
//     num_atribs           = 2 ;     // número de atributos que gestionan los shaders
bool
    redibujar_ventana   = true ,   // puesto a true por los gestores de eventos cuando cambia el modelo y hay que regenerar la vista
    terminar_programa   = false ;  // puesto a true en los gestores de eventos cuando hay que terminar el programa
GLFWwindow *
    ventana_glfw        = nullptr; // puntero a la ventana GLFW
int
    ancho_actual        = 512 ,    // ancho actual del framebuffer, en pixels
    alto_actual         = 512 ;    // alto actual del framebuffer, en pixels
DescrVAO
    * vao_ind          = nullptr , // identificador de VAO (vertex array object) para secuencia indexada
    * vao_no_ind       = nullptr , // identificador de VAO para secuencia de vértices no indexada
    * vao_glm          = nullptr ; // identificador de VAO para secuencia de vértices guardada en vectors de vec3
Cauce 
    * cauce            = nullptr ; // puntero al objeto de la clase 'Cauce' en uso.


/******************************************************************************/
/******************************************************************************/

DescrVAO * dvao = nullptr;

// EJERCICIO 1.1 a)
void generaVerticesPoligono11a(unsigned n) {

    assert( n > 2) ;
    
    std::vector<glm::dvec2> vertices ;
    
    // LINE_LOOP
    for (unsigned i=0; i<n; i++) {
        vertices.push_back(glm::dvec2(cos(2*i*M_PI/n), sin(2*i*M_PI/n)));
    }

    dvao = new DescrVAO(1, new DescrVBOAtribs(0,GL_DOUBLE,2, vertices.size(),&vertices)) ;

}

// EJERCICIO 1.1 b)
void generaVerticesPoligono11b(unsigned n) {

    assert( n > 2) ;
    
    std::vector<glm::dvec2> vertices ;
    
    // LINES
    vertices.push_back(glm::dvec2(cos(2*0*M_PI/n), sin(2*0*M_PI/n)));

    for (unsigned i=1; i<n; i++) {
        vertices.push_back(glm::dvec2(cos(2*i*M_PI/n), sin(2*i*M_PI/n)));
        vertices.push_back(glm::dvec2(cos(2*i*M_PI/n), sin(2*(i*M_PI/n))));
    }

    vertices.push_back(glm::dvec2(cos(2*0*M_PI/n), sin(2*0*M_PI/n)));

    dvao = new DescrVAO(1, new DescrVBOAtribs(0,GL_DOUBLE,2, vertices.size(),&vertices)) ;

}

// EJERCICIO 1.2 a)
void generaVerticesPoligono12a(unsigned n) {

    assert( n > 2) ;
    
    std::vector<glm::vec3> vertices ;
    
    // LINES
    for (unsigned i=0; i<n; i++) {
        vertices.push_back(glm::vec3(0.0f,0.0f,0.0f));
        vertices.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));
        vertices.push_back(glm::vec3(cos(2*(i+1)*M_PI/n), sin(2*(i+1)*M_PI/n), 0.0));
    }

    dvao = new DescrVAO(1, new DescrVBOAtribs(0,vertices)) ;

}

// EJERCICIO 1.2 b)
void generaVerticesPoligono12b(unsigned n) {

    assert( n > 2) ;
    
    std::vector<glm::vec3> vertices ;
    std::vector<glm::uvec3> triangulos ;
    
    // LINES
    vertices.push_back(glm::vec3(0.0f,0.0f,0.0f));

    for (unsigned i=0; i<n; i++) {
        vertices.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));
    }

    for (unsigned i=1; i<n ; i++) {
        triangulos.push_back(glm::vec3(0,i,i+1)) ;
    }

    triangulos.push_back(glm::vec3(0,n,1)) ;

    dvao = new DescrVAO(1, new DescrVBOAtribs(0,vertices)) ;
    dvao -> agregar(new DescrVBOInds(triangulos)) ;

}

// EJERCICIO 1.3 a)

DescrVAO * dvao2 ;

void generaVerticesPoligono13(unsigned n) {

    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    if ( dvao == nullptr )
    {

        // VAO DE RELLENO ( 1.2 b) )

        vector<glm::vec3> vertices ;
        vector<glm::uvec3> triangulos ;

        // LINES
        vertices.push_back(glm::vec3(0.0f,0.0f,0.0f));

        for (unsigned i=0; i<n; i++) {
            vertices.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));
        }

        for (unsigned i=1; i<n ; i++) {
            triangulos.push_back(glm::vec3(0,i,i+1)) ;
        }

        triangulos.push_back(glm::vec3(0,n,1)) ;

        dvao = new DescrVAO(1, new DescrVBOAtribs(0,vertices)) ;
        dvao -> agregar(new DescrVBOInds(triangulos)) ;


        // VAO DE ARISTAS ( 1.1 a))
        vector<dvec2> posiciones ;

        for (unsigned i=0; i<n; i++) {
            posiciones.push_back(glm::dvec2(cos(2*i*M_PI/n), sin(2*i*M_PI/n)));
        }
        
        dvao2 = new DescrVAO(1, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_DOUBLE, 2, posiciones.size(), posiciones.data()));
   
    }

    glVertexAttrib3f(cauce->ind_atrib_colores, 0.1f,0.9f,0.7f) ;
    dvao->draw( GL_TRIANGLES );

    glVertexAttrib3f(cauce->ind_atrib_colores, 0.0f,0.0f,0.0f) ;
    dvao2->draw( GL_LINE_LOOP ) ;

    assert( glGetError() == GL_NO_ERROR );
}

// EJERCICIO 1.4 (35:55)

void generaVerticesPoligono14(unsigned n) {

    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    if ( dvao == nullptr )
    {

        // VAO DE RELLENO ( 1.2 b) )

        vector<glm::vec3> vertices ;
        vector<glm::uvec3> triangulos ;

        // LINES

        for (unsigned i=0; i<n; i++) {
            vertices.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));
        }

        vertices.push_back(glm::vec3(0.0f,0.0f,0.0f));

        for (unsigned i=0; i<n-1 ; i++) {
            triangulos.push_back(glm::vec3(n,i,i+1)) ;
        }

        triangulos.push_back(glm::vec3(n,n-1,0)) ;

        dvao = new DescrVAO(1, new DescrVBOAtribs(0,vertices)) ;
        dvao -> agregar(new DescrVBOInds(triangulos)) ;
    }

    dvao -> crearVAO() ;

    glVertexAttrib3f(cauce->ind_atrib_colores, 0.1f,0.9f,0.7f) ;
    dvao->draw( GL_TRIANGLES );

    glVertexAttrib3f(cauce->ind_atrib_colores, 0.0f,0.0f,0.0f) ;
    dvao->myDraw( GL_LINE_LOOP , n) ;

    assert( glGetError() == GL_NO_ERROR );
}

// EJERCICIO 1.5 

void generaVerticesPoligono15(unsigned n) {

    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    if ( dvao == nullptr )
    {

        // VAO DE RELLENO ( 1.2 b) )

        vector<glm::vec3> vertices ;
        vector<glm::uvec3> triangulos ;

        // LINES

        glm::vec3 color ;
        vector <glm::vec3> colores ;

        for (unsigned i=0; i<n; i++) {
            vertices.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));

            color = {(float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX } ;
            colores.push_back(color) ;
        }

        vertices.push_back(glm::vec3(0.0f,0.0f,0.0f));
        color = {(float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX } ;
        colores.push_back(color) ;



        for (unsigned i=0; i<n-1 ; i++) {
            triangulos.push_back(glm::vec3(n,i,i+1)) ;
        }

        triangulos.push_back(glm::vec3(n,n-1,0)) ;

        dvao = new DescrVAO(2, new DescrVBOAtribs(cauce->ind_atrib_posiciones,vertices)) ;
        dvao -> agregar(new DescrVBOInds(triangulos)) ;
        dvao -> agregar(new DescrVBOAtribs(cauce->ind_atrib_colores, colores)) ;
    }

    dvao -> crearVAO() ;

    dvao->draw( GL_TRIANGLES );

    dvao->habilitarAtrib(cauce->ind_atrib_colores, false) ;
    dvao->myDraw( GL_LINE_LOOP , n) ;

    assert( glGetError() == GL_NO_ERROR );
}

// EJERCICIO 1.6 (AOS)

void generaVerticesPoligono16(unsigned n) {

    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    GLuint array = 0 ;
    GLuint buffer ;

    glm::vec3 color ;
    vector<glm::vec3> vertices_colores ;
    vector<glm::uvec3> triangulos ;

    if ( dvao == nullptr )
    {

        // VAO DE RELLENO ( 1.2 b) )

        // LINES
        for (unsigned i=0; i<n; i++) {
            vertices_colores.push_back(glm::vec3(cos(2*i*M_PI/n), sin(2*i*M_PI/n),0.0));

            color = {(float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX } ;
            vertices_colores.push_back(color) ;
        }

        vertices_colores.push_back(glm::vec3(0.0f,0.0f,0.0f));
        color = {(float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX } ;
        vertices_colores.push_back(color) ;

        // INDICES
        for (unsigned i=0; i<n-1 ; i++) {
            triangulos.push_back(glm::vec3(n,i,i+1)) ;
        }

        triangulos.push_back(glm::vec3(n,n-1,0)) ;

        // CREAMOS VAO
        glGenVertexArrays( 1, &array ); assert( array > 0 );
        glBindVertexArray( array );

        // CREAR VBO
        glGenBuffers( 1, &buffer ); assert( 0 < buffer );
        glBindBuffer( GL_ARRAY_BUFFER, buffer ); 
        glBufferData( GL_ARRAY_BUFFER, 3*sizeof(float)*vertices_colores.size(), vertices_colores.data(), GL_STATIC_DRAW );  

        // POSICIONES
        glVertexAttribPointer( cauce->ind_atrib_posiciones, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glEnableVertexAttribArray(cauce->ind_atrib_posiciones);

        // COLORES 
        glVertexAttribPointer( cauce->ind_atrib_colores, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (GLvoid*)(3*sizeof(float)));
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glEnableVertexAttribArray(cauce->ind_atrib_colores);

        // INDICES
        glGenBuffers( 1, &buffer ); assert( 0 < buffer );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer ); 
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int)*triangulos.size(), triangulos.data(), GL_STATIC_DRAW );

    }

    glDisable( GL_DEPTH_TEST );
    
    glBindVertexArray( array );
    glDrawElements( GL_TRIANGLES, 3*triangulos.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(1) ;
    glDrawArrays( GL_LINE_LOOP, 0, n );

    CError();
    glBindVertexArray( 0 );
}


/******************************************************************************/
/******************************************************************************/



// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar el contenido en la ventana

void VisualizarFrame( )
{
    using namespace std ;
    using namespace glm ;

    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // usar (acrivar) el objeto programa (no es necesario hacerlo en 
    // cada frame si solo hay uno de estos objetos, pero se incluye 
    // para hacer explícito que el objeto programa debe estar activado)
    cauce->activar();

    // establece la zona visible (toda la ventana)
    glViewport( 0, 0, ancho_actual, alto_actual );

    // fija la matriz de transformación de posiciones de los shaders 
    // (la hace igual a la matriz identidad)
    cauce->resetMM();

    // fija la matriz de proyeccion (la hace igual a la matriz identidad)
    cauce->fijarMatrizProyeccion( glm::mat4(1.0) );

    // limpiar la ventana
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // deshabilitar EPO por Z-buffer (test de profundidad)
    glDisable( GL_DEPTH_TEST );

    // AQUÍ LO QUE SE DIBUJA BUSCA AQUI
    // Dibujado
    generaVerticesPoligono16(12) ;
    
    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // esperar a que termine 'glDrawArrays' y entonces presentar el framebuffer actualizado
    glfwSwapBuffers( ventana_glfw );

}


// ---------------------------------------------------------------------------------------------
// función que se invoca cada vez que cambia el número de pixels del framebuffer
// (cada vez que se redimensiona la ventana)

void FGE_CambioTamano( GLFWwindow* ventana, int nuevo_ancho, int nuevo_alto )
{
    using namespace std ;
    //cout << "FGE cambio tamaño, nuevas dimensiones: " << nuevo_ancho << " x " << nuevo_alto << "." << endl ;
    ancho_actual      = nuevo_ancho ;
    alto_actual       = nuevo_alto ;
    redibujar_ventana = true ; // fuerza a redibujar la ventana
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante una tecla.

void FGE_PulsarLevantarTecla( GLFWwindow* ventana, int key, int scancode, int action, int mods )
{
    using namespace std ;
    //cout << "FGE pulsar levantar tecla, número de tecla == " << key << "." << endl ;
    // si se pulsa la tecla 'ESC', acabar el programa
    if ( key == GLFW_KEY_ESCAPE )
        terminar_programa = true ;
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante un botón del ratón

void FGE_PulsarLevantarBotonRaton( GLFWwindow* ventana, int button, int action, int mods )
{
    // nada, por ahora

}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que cambie la posición del puntero

void FGE_MovimientoRaton( GLFWwindow* ventana, double xpos, double ypos )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que mueva la rueda del ratón.

void FGE_Scroll( GLFWwindow* ventana, double xoffset, double yoffset )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cuando se produzca un error de GLFW

void ErrorGLFW ( int error, const char * description )
{
    using namespace std ;
    cerr
        << "Error en GLFW. Programa terminado" << endl
        << "Código de error : " << error << endl
        << "Descripción     : " << description << endl ;
    exit(1);
}
// ---------------------------------------------------------------------------------------------
// código de inicialización de GLFW

void InicializaGLFW( int argc, char * argv[] )
{
    using namespace std ;

    // intentar inicializar, terminar si no se puede
    glfwSetErrorCallback( ErrorGLFW );
    if ( ! glfwInit() )
    {
        cout << "Imposible inicializar GLFW. Termino." << endl ;
        exit(1) ;
    }

    // especificar versión de OpenGL y parámetros de compatibilidad que se querrán
   // (pedimos opengl 330, tipo "core" (sin compatibilidad con versiones anteriores)
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); 
   glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE ); // indica que tambien debe funcionar si se usa con un driver con version superior a la 3.3
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // indica que no es compatible hacia atrás con versiones previas a la 3.3

    // especificar que función se llamará ante un error de GLFW
    glfwSetErrorCallback( ErrorGLFW );

    // crear la ventana (var. global ventana_glfw), activar el rendering context
    ventana_glfw = glfwCreateWindow( 512, 512, "IG ejemplo mínimo (OpenGL 3+)", nullptr, nullptr );
    glfwMakeContextCurrent( ventana_glfw ); // necesario para OpenGL

    // leer y guardar las dimensiones del framebuffer en pixels
    glfwGetFramebufferSize( ventana_glfw, &ancho_actual, &alto_actual );

    // definir cuales son las funciones gestoras de eventos (con nombres 'FGE_....')
    glfwSetFramebufferSizeCallback( ventana_glfw, FGE_CambioTamano );
    glfwSetKeyCallback            ( ventana_glfw, FGE_PulsarLevantarTecla );
    glfwSetMouseButtonCallback    ( ventana_glfw, FGE_PulsarLevantarBotonRaton);
    glfwSetCursorPosCallback      ( ventana_glfw, FGE_MovimientoRaton );
    glfwSetScrollCallback         ( ventana_glfw, FGE_Scroll );
}

// ---------------------------------------------------------------------------------------------
// función para inicializar GLEW (necesario para las funciones de OpenGL ver 2.0 y posteriores)
// en macOS no es necesario (está vacía)

void InicializaGLEW()
{
#ifndef __APPLE__
    using namespace std ;
    GLenum codigoError = glewInit();
    if ( codigoError != GLEW_OK ) // comprobar posibles errores
    {
        cout << "Imposible inicializar ’GLEW’, mensaje recibido: " << endl
             << (const char *)glewGetErrorString( codigoError ) << endl ;
        exit(1);
    }
    else
        cout << "Librería GLEW inicializada correctamente." << endl << flush ;

#endif
}

// ---------------------------------------------------------------------------------------------

void InicializaOpenGL()
{
    using namespace std ;
    
    assert( glGetError() == GL_NO_ERROR );

    cout  << "Datos de versión e implementación de OpenGL" << endl
         << "    Implementación de : " << glGetString(GL_VENDOR)  << endl
         << "    Hardware          : " << glGetString(GL_RENDERER) << endl
         << "    Versión de OpenGL : " << glGetString(GL_VERSION) << endl
         << "    Versión de GLSL   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl ;

    
    InicializaGLEW(); // En linux y windows, fija punteros a funciones de OpenGL version 2.0 o superiores
    
    
    assert( glGetError() == GL_NO_ERROR );
    
    glClearColor( 1.0, 1.0, 1.0, 0.0 ); // color para 'glClear' (blanco, 100% opaco)
    glDisable( GL_CULL_FACE );          // dibujar todos los triángulos independientemente de su orientación
    cauce = new Cauce() ;            // crear el objeto programa (variable global 'cauce')
    
    assert( cauce != nullptr );
    assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BucleEventosGLFW()
{
    while ( ! terminar_programa )
    {   
        if ( redibujar_ventana )
        {   
            VisualizarFrame();
            redibujar_ventana = false; // (evita que se redibuje continuamente)
        }
        glfwWaitEvents(); // esperar evento y llamar FGE (si hay alguna)
        terminar_programa = terminar_programa || glfwWindowShouldClose( ventana_glfw );
    }
}
// ---------------------------------------------------------------------------------------------





// ----------------------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    using namespace std ;
    cout << "Programa mínimo de OpenGL 3.3 o superior" << endl ;

    srand(time(NULL)) ; // Para poder generar números aleatorios

    InicializaGLFW( argc, argv ); // Crea una ventana, fija funciones gestoras de eventos
    InicializaOpenGL() ;          // Compila vertex y fragment shaders. Enlaza y activa programa. Inicializa GLEW.
    BucleEventosGLFW() ;          // Esperar eventos y procesarlos hasta que 'terminar_programa == true'
    glfwTerminate();              // Terminar GLFW (cierra la ventana)

    cout << "Programa terminado normalmente." << endl ;
}
