from distutils.core import setup, Extension

def main():
    setup(name="toy",
          version="1.0.0",
          description="Toy machine package",
          include_dirs=["/usr/include/python3.8"],
          ext_modules=[Extension("toy", ["src/_toy.c"])])

if __name__ == "__main__":
    main()