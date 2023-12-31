//https://dl.radxa.com/rock2/docs/hw/ds/ES8388%20user%20Guide.pdf
//https://docs.google.com/spreadsheets/d/1_UdXtrtpj_dzi36CSwv5tBphtGsBrY2Npdi79h1dYUk/edit#gid=0


#include "es8388_component.h"
#include "esphome/core/hal.h"

#include <soc/io_mux_reg.h>

namespace esphome {
namespace es8388 {

void ES8388Component::setup() {
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
  WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFF0);

  // mute DAC
  this->write_byte(0x19, 0x24);
  // mute ADC
  this->write_byte(0x0F, 0x30);
  // powerup
  this->write_byte(0x01, 0x50);
  this->write_byte(0x02, 0x00);
  // worker mode
  this->write_byte(0x08, 0x00);
  // DAC powerdown
  this->write_byte(0x04, 0xC0);
  // vmidsel/500k ADC/DAC idem
  this->write_byte(0x00, 0x12);

  // i2s 16 bits
  this->write_byte(0x17, 0x18);
  // sample freq 256
  this->write_byte(0x18, 0x02);
  // LIN2/RIN2 for mixer
  this->write_byte(0x26, 0x00);
  // left DAC to left mixer
  this->write_byte(0x27, 0x90);
  // right DAC to right mixer
  this->write_byte(0x2A, 0x90);
  // DACLRC ADCLRC idem
  this->write_byte(0x2B, 0x80);
  this->write_byte(0x2D, 0x00);
  // DAC volume max
  this->write_byte(0x1B, 0x00);
  this->write_byte(0x1A, 0x00);

  // ADC poweroff
  this->write_byte(0x03, 0xFF);

  // ADC amp 24dB original
  //this->write_byte(0x09, 0x88);

  //@nightdav
  this->write_byte(0x09, 0x77); // +21dB : recommended value for ALC & voice recording

  //original
  // LINPUT1/RINPUT1
  //this->write_byte(0x0A, 0x00);

  //@nightdav
  // Mic input : L-R differential
  this->write_byte(0x0A, 0xF0);

  //original
  // ADC mono left
  //this->write_byte(0x0B, 0x02);

  //@nightdav
  // set to Mono Right
  this->write_byte(0x0B, 0x10);


  // i2S 16b
  this->write_byte(0x0C, 0x0C);
  // MCLK 256
  this->write_byte(0x0D, 0x02);
  // ADC Volume
  this->write_byte(0x10, 0x00);
  this->write_byte(0x11, 0x00);

  // ALC OFF  original
  //this->write_byte(0x03, 0x09);
  //this->write_byte(0x2B, 0x80);

  //@nightDav
  // ALC Config (as recommended by ES8388 user guide for voice recording)
  this->write_byte(0x12, 0xe2); // Reg 0x12 = 0xe2 (ALC enable, PGA Max. Gain=23.5dB, Min. Gain=0dB)
  this->write_byte(0x13, 0xa0); // Reg 0x13 = 0xa0 (ALC Target=-1.5dB, ALC Hold time =0 mS)
  this->write_byte(0x14, 0x12); // Reg 0x14 = 0x12(Decay time =820uS , Attack time = 416 uS)
  this->write_byte(0x15, 0x06); // Reg 0x15 = 0x06(ALC mode)
  this->write_byte(0x16, 0xc3); // Reg 0x16 = 0xc3(nose gate = -40.5dB, NGG = 0x01(mute ADC))

  // Power on ADC
  this->write_byte(0x03, 0x09);
  this->write_byte(0x2B, 0x80);


  this->write_byte(0x02, 0xF0);
  delay(1);
  this->write_byte(0x02, 0x00);
  // DAC power-up LOUT1/ROUT1 and LOUT2/ROUT2 enabled
  this->write_byte(0x04, 0x3C);
  this->write_byte(0x03, 0x00);

  // DAC volume max
  this->write_byte(0x2E, 0x1C);
  this->write_byte(0x2F, 0x1C);
  // Headphone volume max
  this->write_byte(0x30, 0x1C);
  this->write_byte(0x31, 0x1C);
  // unmute ADC with fade in
  this->write_byte(0x0F, 0x60);
  // unmute with DAC fade in
  this->write_byte(0x19, 0x20);
}

}  // namespace es8388
}  // namespace esphome
