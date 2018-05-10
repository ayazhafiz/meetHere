{
    'variables': {
        'FLAGS':
            '-Wall -Werror -Wextra\
             -Wno-unused-parameter\
             -Wshadow -Wfloat-equal -Wpointer-arith\
             -Wstrict-prototypes\
             -pedantic -pedantic-errors\
             -O2'

    },
    'targets': [
        {
            'target_name': '__c',
            'sources': [
                '<!@(ls -1 src/native/*.c)',
                '<!@(ls -1 src/native/toolkit/*.c)',
            ],
            'cflags': ['-std=c99 <(FLAGS)'],
            'xcode_settings': {
                'OTHER_CFLAGS': ['-std=c99', '<(FLAGS)'],
                'MACOSX_DEPLOYMENT_TARGET': '10.10'
            }
        },
        {
            'target_name': 'api',
            'sources': [
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/toolkit/array.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/toolkit/ips.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/toolkit/matrix.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/cartesian.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/point_set.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/polynomial.o',
                '<(module_root_dir)/build/Release/obj.target/__c/src/native/tsp.o',
                '<!@(ls -1 src/native/wrapper/*.cpp)',
            ],
            'cflags': ['-std=c++11'],
            'xcode_settings': {
                'OTHER_CFLAGS': ['-std=c++11',  '-stdlib=libc++'],
                'OTHER_LDFLAGS': ['-stdlib=libc++'],
                'MACOSX_DEPLOYMENT_TARGET': '10.10'
            }
        }
    ]
}
