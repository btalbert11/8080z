#include "pch.h"
#include "GL/freeglut.h"



// buffer to hold the pixels that opengl is going to display
GLubyte image_bitmap[z_DISPLAY_BUFFER_SIZE];
GLubyte* image_bit;


void init_array() {
	for (int i = 0; i < z_DISPLAY_BUFFER_SIZE; i++) {
		image_bitmap[i] = 0x00;
	}
}

void debug_info(State8080 state) {
	std::ofstream output_file("State_dump.txt", std::ios::trunc);
	state.dump(output_file);
}

void init() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display() {


	// might be it
	glPointSize(1);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 224.0, 256.0, 0.0);


	image_bitmap[1] = 0xff;
	image_bitmap[2] = 0xff;
	image_bitmap[3] = 0xff;
	image_bitmap[4] = 0xff;
	image_bitmap[5] = 0xff;
	image_bitmap[6] = 0xff;
	image_bitmap[7] = 0xff;
	image_bitmap[8] = 0xff;
	image_bitmap[9] = 0xff;

	image_bitmap[200] = 0xff;
	image_bitmap[201] = 0xff;
	image_bitmap[202] = 0xff;
	image_bitmap[203] = 0xff;
	image_bitmap[204] = 0xff;

	image_bitmap[500] = 0xff;
	image_bitmap[501] = 0xff;
	image_bitmap[502] = 0xff;
	image_bitmap[503] = 0xff;
	image_bitmap[504] = 0xff;


	glColor3f(1, 1, 1);
	uint8_t pixel_block = 0;
	uint8_t pixel = 0;
	for (int i = 0; i < z_DISPLAY_BUFFER_SIZE; i++) {
		pixel_block = (uint8_t)image_bit[i];
		// handle byte
		for (int j = 0; j < 8; j++) {
			pixel = (pixel_block >> (7 - j)) & 1;
			if (pixel == 1) {
				glBegin(GL_POINTS);
				// map pixel in memory to pixel on screen
				glVertex2i(((i * 8) + j) % 224, floor(((i*8) + j) / 224));
				glEnd();
			}
		}
	}

	glFlush();
	//glutPostRedisplay();





	// Environment test
	//
	//glBegin(GL_QUADS);
	//	glColor3f(1.0f, 0.0f, 0.0f);
	//	glVertex2f(-0.5f, -0.5f);
	//	glVertex2f(-0.5f, 0.5f);
	//	glVertex2f(0.5f, 0.5f);
	//	glVertex2f(0.5f, -0.5f);
	//glEnd();

	//glFlush();

}

int main(int argc, char ** argv)
{

	// Open File and load the program into memory
	std::ifstream f(argv[1], std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		printf("Error opening %s\n", argv[1]);
		exit(1);
	}
	std::streamsize size = f.tellg();
	f.seekg(0, std::ios::beg);

	std::vector<uint8_t> buffer(65535); // Size of 16 bit memory address space

	if (!f.read((char *)buffer.data(), size)) {
		std::cout << "Program failed to load into memory\n";
		exit(-1);
	}
	f.close();

	// initialize classes
	State8080 state = State8080(buffer);
	MachineSI machine;

	int i = 0;
	int p = 0;
	//std::ofstream debug_file("debug output.txt");

	double last_interrupt = 0;

	// Setup opengl window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(224, 256);

	glutCreateWindow("TEST");

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 224.0, 256.0, 0.0);

	// make bitmap pointer
	image_bit = state.memory.data() + 9216;

	glutDisplayFunc(display);
	init();
	init_array();


	//Emulate program
	while (1) {

		// Dump state and machine info every thousand instructions
		i++;
		if (i >= 1000) {
			state.dump_state(std::cout);
			machine.dump_info(std::cout);
			i = 0;
		}
		// poll keyboard, change machine state
		PlatformWindows::poll_keyboard(machine);
		//emulate a single instruction
		state.emulate(machine);


		//generate an interrupt every 1/60th of a second (16.6667 ms)
		if (PlatformWindows::time() - last_interrupt > 1.0 / 60.0) {
			if (state.interrupts_enable) {
				state.generate_interrupt(2);
				last_interrupt = PlatformWindows::time();
			}
		}

		// run 1 iteration of glut loop
		glutMainLoopEvent();
	}



	// Dump emulator info
	debug_info(state);
	return 1;
}
