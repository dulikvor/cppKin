from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
import pathlib, os, shutil, glob

class CMakeExtension(Extension):
    def __init__(self, name):
        # don't invoke the original build_ext for this special extension
        super().__init__(name, sources=[])

class cmake_build_ext(build_ext):
    def run(self):
        for ext in self.extensions:
            if not isinstance(ext, CMakeExtension):
                raise TypeError('ext type {0} mismatch intended CMakeExtension'.format(type(ext)))
            self.build_cmake(ext)

    def build_cmake(self, ext):
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        archive_out_dir = str(os.path.join(extdir, 'cppkin'))

        config = 'Debug' if self.debug else 'Release'
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + archive_out_dir,
            '-D3RD_PARTY_INSTALL_STEP=ON',
            '-DCOMPILATION_STEP=ON',
			'-DWITH_PYTHON=ON',
            '-DPYTHON_BINDING=pyBind',
            '-DCMAKE_BUILD_TYPE=' + config,
            '-DFORCE_INSTALL_RPATH=ON'
        ]
        cwd = pathlib.Path().absolute()
        build_temp_rel = os.path.relpath(cwd, build_temp)
        os.chdir(str(build_temp))
        self.spawn(['cmake', str(build_temp_rel)] + cmake_args)
        self.spawn(['make'])
        os.chdir(str(cwd))
        for file in glob.glob(str(build_temp / 'Third_Party' / 'lib') + '/*.so'):
            shutil.copy2(file, archive_out_dir)

setup(
    name='cppkin',
    version='1.1.2',
    description='zipkin tracing library',
    url='https://github.com/Dudi119/cppKin',
    author='David (Dudi) Likvornik',
    author_email='likdudi@gmail.com',
    license='Apache 2.0',
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ],
    keywords='zipkin instrumentation tracing',
    packages=['cppkin'],
    ext_modules=[CMakeExtension('cppKin')],
    include_package_data=True,
    cmdclass={
        'build_ext': cmake_build_ext
    },
    long_description="""
    cppkin is an instrumentation client library for zipkin available in C++ and Python.
    """
 )
