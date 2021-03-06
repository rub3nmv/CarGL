#include "GuiManager.h"

#include "Scene.h"
#include "Object.h"
#include <GL/glui.h>

#include <iostream>

// IDs para los callbacks de TGui
#define LISTBOX_CAMERAS			100

#define CAR_COLOR				270

#define ENABLE_ID				300
#define DISABLE_ID				301
#define RESET_ID				400

#define SHADING_FLAT			500
#define SHADING_SMOOTH			501
#define PERSPECTIVE				502
#define PARALLEL				503
#define CLOCKWISE				504
#define CCLOCKWISE				505

#define TRANSLATE_FACTOR		0.4
#define ROTATE_FACTOR			1.5
#define SCALE_FACTOR			0.05

GuiManager* guiMan;

// By-pass para el controllCallback de tipo ANSI C
void controlCallback( int control )
{
    guiMan->ControlCallback(control);
}

GuiManager::GuiManager(Scene* scene)
{
	guiMan = this;
	this->scene = Scene::instance();
	scene->guiManager = this;

	listboxCamarasCount = 0;

	enable_panel2 = 0;

	input.mouseX = 0;
	input.mouseY = 0;
	input.buttonDown = -1;
}

GuiManager::~GuiManager()
{
	scene = 0;
}

void GuiManager::addLightItem(Light* light)
{
	GLUI_Rollout* luz = new GLUI_Rollout( rolloutLuces, light->name, false );
	new GLUI_Checkbox( luz, "Enabled", &light->enabled );
	new GLUI_StaticText( luz, "Intensidad" );
	GLUI_Scrollbar* sb = new GLUI_Scrollbar( luz, "Intensity", GLUI_SCROLL_HORIZONTAL,
                            &light->intensity);

    sb->set_float_limits(0,1);
    new GLUI_StaticText( luz, "Rojo" );
	sb = new GLUI_Scrollbar( luz, "Rojo", GLUI_SCROLL_HORIZONTAL,
                            &light->diffuse[0]);
    sb->set_float_limits(0,1);
    new GLUI_StaticText( luz, "Verde" );
    sb = new GLUI_Scrollbar( luz, "Verde", GLUI_SCROLL_HORIZONTAL,
                            &light->diffuse[1]);
    sb->set_float_limits(0,1);
    new GLUI_StaticText( luz, "Azul" );
    sb = new GLUI_Scrollbar( luz, "Azul", GLUI_SCROLL_HORIZONTAL,
                            &light->diffuse[2]);
    sb->set_float_limits(0,1);
}

void GuiManager::addCarItem(Object* car)
{
	GLUI_Rollout* luz = new GLUI_Rollout( rolloutCoches, car->name.c_str(), false );

    new GLUI_StaticText( luz, "Rojo" );
	GLUI_Scrollbar* sb = new GLUI_Scrollbar( luz, "Rojo", GLUI_SCROLL_HORIZONTAL, &car->color[0]);
    sb->set_float_limits(0,1);
    new GLUI_StaticText( luz, "Verde" );
    sb = new GLUI_Scrollbar( luz, "Verde", GLUI_SCROLL_HORIZONTAL, &car->color[1]);
    sb->set_float_limits(0,1);
    new GLUI_StaticText( luz, "Azul" );
    sb = new GLUI_Scrollbar( luz, "Azul", GLUI_SCROLL_HORIZONTAL, &car->color[2]);
    sb->set_float_limits(0,1);
}

void GuiManager::addCameraItem(const char* name, bool active)
{
	listboxCamaras->add_item(listboxCamarasCount, name);
	if ( active )
	{
		listboxCamaras->do_selection(listboxCamarasCount);
		controlCallback(LISTBOX_CAMERAS);
	}
	listboxCamarasCount++;
}

void __fastcall GuiManager::init(int main_window) {

	guiMan = this;

    /****************************************************/
    /*        Codigo con el interfaz Grafico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", false );

    /***** Control para las propiedades de scene *****/
    new GLUI_Checkbox( obj_panel, "Dibujar reflejos (R)", &scene->show_reflections );
    new GLUI_Checkbox( obj_panel, "Luz ambiente (L)", &(scene->ambientLighting) );
    new GLUI_Checkbox( obj_panel, "Modo alambrico (W)", &(scene->wireframe) );
    new GLUI_Checkbox( obj_panel, "Z Buffer (Z)", &scene->zbuffer);
    new GLUI_Checkbox( obj_panel, "Culling (C)", &scene->culling );
    new GLUI_Checkbox( obj_panel, "Texturas (T)", &scene->textures );
    new GLUI_Checkbox( obj_panel, "Mipmapping (M)", &scene->mipmapping );
    new GLUI_StaticText( obj_panel, "Modelo de sombreado" );
    new GLUI_Button( obj_panel, "Flat", SHADING_FLAT, controlCallback );
    new GLUI_Button( obj_panel, "Smooth", SHADING_SMOOTH, controlCallback );
    new GLUI_StaticText( obj_panel, "Proyeccion" );
    new GLUI_Button( obj_panel, "Perspectiva", PERSPECTIVE, controlCallback );
    new GLUI_Button( obj_panel, "Paralela", PARALLEL, controlCallback );
    new GLUI_StaticText( obj_panel, "Sentido caras exteriores" );
    new GLUI_Button( obj_panel, "Horario", CLOCKWISE, controlCallback );
    new GLUI_Button( obj_panel, "Antihorario", CCLOCKWISE, controlCallback );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Objetos", false );
    new GLUI_Checkbox( options, "Dibujar Skybox", &scene->show_skybox );
    new GLUI_Checkbox( options, "Dibujar Coche", &scene->show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &scene->show_ruedas );
    new GLUI_Checkbox( options, "Dibujar Carretera", &scene->show_carretera );
    new GLUI_Checkbox( options, "Dibujar Acera", &scene->show_acera );
    new GLUI_Checkbox( options, "Dibujar Farolas", &scene->show_farolas );
    new GLUI_Checkbox( options, "Dibujar Edificios", &scene->show_edificios );
    new GLUI_Checkbox( options, "Dibujar Papeleras", &scene->show_papeleras );
    new GLUI_Checkbox( options, "Dibujar Bancos", &scene->show_bancos );
    new GLUI_Checkbox( options, "Dibujar Rotonda", &scene->show_rotonda );
    new GLUI_Checkbox( options, "Dibujar Senales", &scene->show_senales );

    /*** Coches ***/
    rolloutCoches = new GLUI_Rollout( glui, "Coches", false );

	/*** Luces ***/
    rolloutLuces = new GLUI_Rollout( glui, "Luces", false );

    /*** Camaras ***/
	listboxCamaras = new GLUI_Listbox(glui, "Camaras", 0, LISTBOX_CAMERAS, controlCallback );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Bloquear Movimiento", &enable_panel2 );
    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/

    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "Autor: Ruben Martinez Vilar. 2013-2014" );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    // A�ade un panel con texto con el valor de la selecci�n
    GLUI_Panel *panel0 = new GLUI_Panel( glui, "Seleccion");
    sel_tex = new GLUI_StaticText( panel0, "0");

    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

	// Rotacion en todas las direcciones
    view_rot = new GLUI_Rotation(glui2, "Rotacion escena", scene->view_rotate );
    //view_rot->set_spin( 0.5 );

    new GLUI_Column( glui2, false );
    trans_xy = new GLUI_Translation(glui2, "Traslacion escena XY", GLUI_TRANSLATION_XY, scene->view_position );
    trans_xy->set_speed( .05 );

    new GLUI_Column( glui2, false );
    trans_x =  new GLUI_Translation(glui2, "Traslacion escena X", GLUI_TRANSLATION_X, scene->view_position );
    trans_x->set_speed( .05 );

    new GLUI_Column( glui2, false );
    trans_y = new GLUI_Translation( glui2, "Traslacion escena Y", GLUI_TRANSLATION_Y, &scene->view_position[1] );
    trans_y->set_speed( .05 );

    new GLUI_Column( glui2, false );
    trans_z = new GLUI_Translation( glui2, "Traslacion escena Z", GLUI_TRANSLATION_Z, &scene->view_position[2] );
    trans_z->set_speed( .05 );

    new GLUI_Column( glui2, false );
    zoom = new GLUI_Translation( glui2, "Escalado escena", GLUI_TRANSLATION_Z, &scene->scale );
    zoom->set_speed( .05 );
}

void __fastcall GuiManager::ControlCallback( int control )
{
	switch (control)
	{
		case ENABLE_ID:
			glui2->enable();
			break;
		case DISABLE_ID:
			glui2->disable();
			break;
		case RESET_ID:
			scene->setCamera(listboxCamaras->get_int_val());
			// Al parecer la GLUI_TRANSLATION_Z es la unica traslacion que
			// no se resetea correctamente con la scale de la escena
			zoom->set_z(scene->scale);
			view_rot->reset();
			break;
		case SHADING_FLAT:
			scene->smooth_shading = 0;
			break;
		case SHADING_SMOOTH:
			scene->smooth_shading = 1;
			break;
		case PERSPECTIVE:
			scene->perspective = 1;
			scene->reshape(0,0);
			break;
		case PARALLEL:
			scene->perspective = 0;
			scene->reshape(0,0);
			break;
		case CLOCKWISE:
			scene->clockwise = 1;
			break;
		case CCLOCKWISE:
			scene->clockwise = 0;
			break;
		case LISTBOX_CAMERAS:
			scene->setCamera(listboxCamaras->get_int_val());
			zoom->set_z(scene->scale);
			view_rot->reset();
			break;
	}
}

void __fastcall GuiManager::Idle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != window_id )
		glutSetWindow(window_id);

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
							   application  */
	if (enable_panel2 == 0)
		glui2->enable();
	else
		glui2->disable();

	glutPostRedisplay();
}

void __fastcall GuiManager::Mouse(int button, int button_state, int x, int y )
{
	if(button_state == GLUT_DOWN)
	{
		input.buttonDown = button;
	}
	else
	{
		input.buttonDown = -1;
	}

	input.specialKey = glutGetModifiers();

    scene->pick3D(x, y);
}

// Detecta movimiento de raton
void __fastcall GuiManager::Motion(int x, int y)
{
	if(input.buttonDown >= 0)
	{
		// control pulsado
		if(input.specialKey == GLUT_ACTIVE_CTRL)
		{
			// Rotacion en z
			if(input.buttonDown == 0) // boton izquierdo
			{
				if(input.mouseX < x)
					scene->view_rotation.z += ROTATE_FACTOR;
				else if(input.mouseX > x)
					scene->view_rotation.z -= ROTATE_FACTOR;
			}
			// Escalado en x,y,z
			else if(input.buttonDown == 2)
			{
				if(input.mouseY < y)
					scene->scale -= SCALE_FACTOR;
				else if(input.mouseY > y)
					scene->scale += SCALE_FACTOR;
			}

		}
		else if(input.specialKey == GLUT_ACTIVE_SHIFT)
		{
			// Traslacion x,y
			if(input.buttonDown == 0) // boton izquierdo
			{
				// x
				if(input.mouseX < x )
					scene->view_position[0] += TRANSLATE_FACTOR;
				else if(input.mouseX > x )
					scene->view_position[0] -= TRANSLATE_FACTOR;
				// y
				if(input.mouseY < y)
					scene->view_position[1] -= TRANSLATE_FACTOR;
				else if(input.mouseY > y)
					scene->view_position[1] += TRANSLATE_FACTOR;
			}
		}
		else if(input.specialKey == 3) // no encuentro enumerado para shift+control, pero glutGetModifiers devuelve 3
		{
			// Traslacion z
			if(input.buttonDown == 0) // boton izquierdo
			{
				if(input.mouseY < y)
					scene->view_position[2] -= TRANSLATE_FACTOR;
				else if(input.mouseY > y)
					scene->view_position[2] += TRANSLATE_FACTOR;
			}
		}
		else // Rotaciones y escalado
		{
			// Rotacion en x, y
			if(input.buttonDown == 0)
			{
				if(input.mouseX < x ) // Cuando el raton se mueve en x, queremos girar en y
					scene->view_rotation.y -= ROTATE_FACTOR;
				else if(input.mouseX > x )
					scene->view_rotation.y += ROTATE_FACTOR;
				// y
				if(input.mouseY < y) //Cuando el raton se mueve en y, queremos girar en x
					scene->view_rotation.x += ROTATE_FACTOR;
				else if(input.mouseY > y)
					scene->view_rotation.x -= ROTATE_FACTOR;
			}
			// Escalado
			else if(input.buttonDown == 2)
			{
				if(input.mouseY < y)
					scene->scale -= SCALE_FACTOR;
				else if(input.mouseY > y)
					scene->scale += SCALE_FACTOR;
			}
		}
	}

	input.mouseX = x;
	input.mouseY = y;
}
