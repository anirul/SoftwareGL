#include <gtest/gtest.h>
#include "Camera_test.h"
#include "Image_test.h"
#include "Mesh_test.h"
#include "Renderer_test.h"
#include "Scene_test.h"
#include "Triangle_test.h"
#include "VectorMath_test.h"
#include "Vertex_test.h"

int main(int ac, char** av) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}