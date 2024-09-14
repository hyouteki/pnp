#ifndef PLUG_HELPER_H_
#define PLUG_HELPER_H_

#define Error(msg, ...) ({printf("%s:%d:0: function: %s\nerror: "msg"\n", \
				__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); exit(EXIT_FAILURE);})

#define Error_Usage(msg, ...) ({printf("%s:%d:0: function: %s\nerror: "msg"\n", \
				__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); usage(); exit(EXIT_FAILURE);})

#define Error_Toml(msg, toml_error_msg) ({printf("%s:%d:0: function: %s\nerror: %s\nreason: %s\n", \
				msg, toml_error_msg, __FILE__, __LINE__, __FUNCTION__); exit(EXIT_FAILURE);})

#endif // PLUG_HELPER_H_
