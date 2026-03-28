from conan import ConanFile
from conan.tools.cmake import CMake
import os

# Conan install & build (terminal):
# conan install . --build=missing 
# conan build .

class CatanConan(ConanFile):
    name = "catan"
    version = "0.1"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    options = {
        "with_client": [True, False],
        "with_server": [True, False],
        "with_tests": [True, False],
    }
    default_options = {
        "with_client": True,
        "with_server": True,
        "with_tests": True,
    }

    def requirements(self):
        self.requires("protobuf/[>=5 <6]") # always required

        if self.options.with_tests:
            self.requires("catch2/[>=3.3 <4]") # only for tests

    def layout(self):
        self.folders.build = "build"
        self.folders.generators = os.path.join("build", "generators")

    def build(self):
        cmake = CMake(self)
        with_client = bool(self.options.with_client)
        with_server = bool(self.options.with_server)
        with_tests = bool(self.options.with_tests)
        with_network_qt = with_client or with_server
        cmake.configure(variables={
            "BUILD_CLIENT": with_client,
            "BUILD_SERVER": with_server,
            "BUILD_NETWORK_QT": with_network_qt,
            "BUILD_TESTS": with_tests,
        })
        cmake.build()

