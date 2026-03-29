from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
import os

# Conan install & build:
# conan install . --build=missing
# conan build .

# Conan create package:
# conan create . --build=missing
class CatanConan(ConanFile):
    name = "catan"
    version = "0.1"

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = (
        "CMakeLists.txt",
        "libs/*",
        "game/*",
        "network/*",
        "gui/*",
        "controller/*",
        "appClient/*",
        "appServer/*",
        "tests/*",
        "cmake/*",
        "resources/*",
    )
    options = {
        "with_client": [True, False],
        "with_server": [True, False],
        "with_tests": [True, False],
    }
    default_options = {
        "with_client": True,
        "with_server": True,
        "with_tests": False,
    }
    def generate(self):
        tc = CMakeToolchain(self)

        qt_path = self.conf.get("user.qt:path", default=None) or os.environ.get("QT_DIR")
        if qt_path:
            tc.variables["CMAKE_PREFIX_PATH"] = f"{qt_path};${{CMAKE_PREFIX_PATH}}"

        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()
    def requirements(self):
        self.requires("protobuf/[>=5 <6]") # always required

        if self.options.with_tests:
            self.requires("catch2/[>=3.3 <4]") # only for tests
    def build_requirements(self):
        self.tool_requires("protobuf/<host_version>")
    def layout(self):
        cmake_layout(self, src_folder=".")
    def build(self):
        cmake = CMake(self)
        
        with_client = bool(self.options.with_client)
        with_server = bool(self.options.with_server)
        with_tests = bool(self.options.with_tests)

        cmake.configure(
            variables={
                "BUILD_CLIENT": with_client,
                "BUILD_SERVER": with_server,
                "BUILD_NETWORK_QT": with_client or with_server,
                "BUILD_TESTS": with_tests,
            }
        )
        cmake.build()
    def package(self):
        cmake = CMake(self)
        cmake.install()
    def package_info(self):
        self.cpp_info.resdirs = ["resources"]
