#include <gtest/gtest.h>
#include "Buffer_test.h"
#include "Device_test.h"
#include "Mesh_test.h"
#include "Program_test.h"
#include "Shader_test.h"
#include "Texture_test.h"

int main(int ac, char** av) 
{
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}