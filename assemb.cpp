#include <stdio.h>
#include <sys/stat.h>

#include "assemb.h"

static const size_t commands_amount = 18;

StkErrors file_transformation(const char* const name, Proc* const processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    make_init_buf (name, processor, err);
    
    processing_text(processor, err);  

    output_file (processor, err);

    return ALL_RIGHT;
}

StkErrors make_init_buf (const char* const  name, Proc* const processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    FILE* file; 
    file = fopen(name, "rb");
    if (file == nullptr)
    {
        printf("file wasnt opened\n");
        return FILE_PROBLEM;
    }

    struct stat buf;
    stat(name, &buf);
    size_t init_file_size = buf.st_size;

    char* file_buf = (char*)calloc(init_file_size, sizeof(char));
    if (file_buf == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    fread(file_buf, sizeof(char), init_file_size, file); //ну все, к этому моменту все элементы изначального файла в этом буффере
    
    fclose(file);

    processor->file_buf = file_buf;
    processor->init_file_size = init_file_size;

    return ALL_RIGHT;
}

StkErrors processing_text(Proc* const processor, StkErrors* err)
{
    assert(processor != nullptr);

    char* text = 0;
    size_t size = 0;

    processor->file_buf = text;
    processor->init_file_size = size;

    size_t symbol = 0;
    size_t space_cnt = 0;
    size_t str_cnt = 0;

    while (symbol < size)
    {
        char ch = text[symbol];

        if (ch == '\r')
            ch = '\0';

        else if (ch == '\n')
        {
            ch = '\0';
            str_cnt++;
        }
        else if (ch == ' ')
        {
            ch = '\0';   //здесть тоже надо, команды ведь как строки будут анализироваться
            space_cnt++;
        }
        
        symbol++;
    }
    space_cnt = space_cnt + str_cnt;
    processor->input_file_commands_amount = space_cnt;

    return ALL_RIGHT;
}

StkErrors output_file(Proc* const processor, StkErrors* err)
{
    assert(processor != nullptr);

    size_t input_file_commands_amount = 0;

    processor->input_file_commands_amount = input_file_commands_amount;

    int* new_file_buf = (int*)calloc(input_file_commands_amount, sizeof(int));
    if (new_file_buf == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    cool_compile(processor, new_file_buf, err);

    FILE* output_file;
    output_file = fopen("out.txt", "w");
    if (output_file == nullptr)
    {
        printf("file wasnt opened\n");
        return FILE_PROBLEM;
    }
    
    fwrite(new_file_buf, sizeof(int), input_file_commands_amount, output_file); //крууто, теперь у нас есть файл с нужным содержимым
    
    fclose(output_file);

    processor->new_file_buf = new_file_buf;

    return ALL_RIGHT;
}

StkErrors cool_compile(int* const new_file_buf, Proc* const processor, StkErrors* err)
{
    assert(new_file_buf != nullptr);
    assert(processor != nullptr);

    size_t input_file_commands_amount = processor->input_file_commands_amount;
    char* file_buf = processor->file_buf;
    struct CommandParameters* bunch_of_commands = processor->bunch_of_commands;

    for (size_t i = 0; i < input_file_commands_amount; i++)
    {
        char* str = 0;
        sscanf(file_buf, %s, &str);  //рассчет на то, что он будет считывать - как и обыйчный сканф -  из файла как из потока ввода и будет останавливаться и идти как надо - читывать по одному элементу строки до пробела 
        
        for (size_t com = 0; com < commands_amount; com++)
        {
            if (strcmp(str, bunch_of_commands[com].com_str) == 0)  //strcmp ну никак не избежать
            {
                new_file_buf[i] = bunch_of_commands[com].com_num;  //заполнил макросом, все круто

                if (com == 1)  //перечислить все функции, требующие доп агрумента
                {
                    i++;
                    (int)str = 0;
                    sscanf(file_buf, %d, &str);
                    new_file_buf[i] = str;
                }
            }
        }
    }

    return ALL_RIGHT;
}

