#include "cpu.h"
#include "spi_master.h"
#include "display.h"
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

#define PUT_CHAR(chr) do { spi_write(DISP_SYMBOL_ADDR, (chr)); } while (0)
#define MOVE_CURSOR(pos) do { spi_write(DISP_CURSOR_ADDR, (pos)); } while (0)
#define EXEC_CMD(cmd) do { spi_write(DISP_CMD_REG_ADDR, (cmd)); } while (0)

CPU::CPU(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bo("addr_bo"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    wr_o("wr_o"),
    rd_o("rd_o")
{
    addr_bo.initialize(0);
    data_bo.initialize(0);
    wr_o.initialize(0);
    rd_o.initialize(0);
    
    SC_CTHREAD(mainThread, clk_i.pos());
    
}

CPU::~CPU()
{

}

void CPU::mainThread()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d.%m.%Y");
    auto date = oss.str();

    oss.str("");
    oss << std::put_time(&tm, "%H:%M:%S");
    auto time = oss.str();

    // cout << "[" << date << "]" << " " << "[" << time << "]" << endl;
    
    EXEC_CMD(DISP_CLEAR_CMD);
    for (size_t i = 0; i < date.length(); ++i) {
        PUT_CHAR(date[i]);
    }
    MOVE_CURSOR(DISPLAY_WIDTH);
    for (size_t i = 0; i < date.length(); ++i) {
        PUT_CHAR(time[i]);
    }
    EXEC_CMD(DISP_UPDATE_CMD);
    
    sc_stop();

}

void CPU::spi_write(int addr, int data)
{
    int wd_ctr = 0;
    bool busy_flag = false;
    int data_send = (addr << SPI_ADDR_SIZE) | (data & 0x00FF);

    // send data
    bus_write(DATA_SEND_ADDR, data_send);
    
    // start trans
    bus_write(CMD_REG_ADDR, CMD_SEND_BIT);

    busy_flag = bus_read(CMD_REG_ADDR) & CMD_BUSY_BIT;
    while(busy_flag) {
        wd_ctr++;
        if(wd_ctr == 100) break;
        busy_flag = bus_read(CMD_REG_ADDR) & CMD_BUSY_BIT;
    }

}

int CPU::spi_read(int addr)
{

}

int CPU::bus_read(int addr)
{
    int data;

    wait();
    addr_bo.write(addr);
    rd_o.write(1);
    
    wait();
    rd_o.write(0);
    
    wait();
    data = data_bi.read();
    
    //cout << "CPU: READ " << endl;
    //cout << "  -> addr: " << hex << addr << endl;
    //cout << "  -> data: " << hex << data << endl;
    
    return data;
    
}

void CPU::bus_write(int addr, int data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);
    
    wait();
    wr_o.write(0);
    
    //cout << "CPU: WRITE " << endl;
    //cout << "  -> addr: " << hex << addr << endl;
    //cout << "  -> data: " << hex << data << endl;

}
