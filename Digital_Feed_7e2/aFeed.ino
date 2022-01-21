//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ********** Режим "Асинхронная Подача" ********** //////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aFeed_Left(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;

   Joy_X_flag = OFF;
   feed_X_flag = OFF;
   Start_Speed = (250000 / ((uint32_t)MIN_aFEED * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) - 1) /FEED_ACCEL;
   if (Motor_Z_Pos < Limit_Pos_Left - Start_Speed * 2)
   {
      aFeed_Divisor = 250000 / ((uint32_t)aFeed_mm * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) - 1;
      if (aFeed_Divisor < Start_Speed)
      {
         max_OCR4A = Start_Speed;
         if (Step_Z_flag == OFF)
         {
            OCR4A = Start_Speed;
         }
      }
      else
      {
         max_OCR4A = aFeed_Divisor;
         OCR4A = aFeed_Divisor;
      }
   }
   else
   {
      aFeed_Divisor = Start_Speed;
      max_OCR4A = Start_Speed;
      OCR4A = Start_Speed;
   }
   
   Brake_Compens = (max_OCR4A - aFeed_Divisor) +1;
   Limit_Pos = Limit_Pos_Left - Brake_Compens;

   Motor_Z_Dir = ZCW;
   Motor_Z_CW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();
   
   feed_Z_flag = ON;
   Joy_Z_flag = ON;
   Ena_INT_Z_aFeed();
}

/////////////////////////////////////////////
void aFeed_Right(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;

   Joy_X_flag = OFF;
   feed_X_flag = OFF;
   Start_Speed = (250000 / ((uint32_t)MIN_aFEED * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) - 1) /FEED_ACCEL;
   if (Motor_Z_Pos > Limit_Pos_Right + Start_Speed * 2)
   {
      aFeed_Divisor = 250000 / ((uint32_t)aFeed_mm * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) - 1;
      if (aFeed_Divisor < Start_Speed)
      {
         max_OCR4A = Start_Speed;
         if (Step_Z_flag == OFF)
         {
            OCR4A = Start_Speed;
         }
      }
      else
      {
         max_OCR4A = aFeed_Divisor;
         OCR4A = aFeed_Divisor;
      }
   }
   else
   {
      aFeed_Divisor = Start_Speed;
      max_OCR4A = Start_Speed;
      OCR4A = Start_Speed;
   }
   
   Brake_Compens = (max_OCR4A - aFeed_Divisor) +1;
   Limit_Pos = Limit_Pos_Right + Brake_Compens;

   Motor_Z_Dir = ZCCW;
   Motor_Z_CCW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();
   
   feed_Z_flag = ON;
   Joy_Z_flag = ON;
   Ena_INT_Z_aFeed();
}

void aFeed_Front(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;

   Joy_Z_flag = OFF;
   feed_Z_flag = OFF;
   Start_Speed = (250000 / ((uint32_t)MIN_aFEED * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) - 1) /FEED_ACCEL;
   if (Motor_X_Pos < Limit_Pos_Front - Start_Speed * 2)
   {
      aFeed_Divisor = 250000 / ((uint32_t)aFeed_mm * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) - 1;
      if (aFeed_Divisor < Start_Speed)
      {
         max_OCR4A = Start_Speed;
         if (Step_X_flag == OFF)
         {
            OCR4A = Start_Speed;
         }
      }
      else
      {
         max_OCR4A = aFeed_Divisor;
         OCR4A = aFeed_Divisor;
      }
   }
   else
   {
      aFeed_Divisor = Start_Speed;
      max_OCR4A = Start_Speed;
      OCR4A = Start_Speed;
   }
   
   Brake_Compens = (max_OCR4A - aFeed_Divisor) +1;
   Limit_Pos = Limit_Pos_Front - Brake_Compens;

   Motor_X_Dir = CW;
   Motor_X_CW();
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   feed_X_flag = ON;
   Joy_X_flag = ON;
   Ena_INT_X_aFeed();
}

void aFeed_Rear(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;

   Joy_Z_flag = OFF;
   feed_Z_flag = OFF;
   Start_Speed = (250000 / ((uint32_t)MIN_aFEED * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) - 1) /FEED_ACCEL;
   if (Motor_X_Pos > Limit_Pos_Rear + Start_Speed * 2)
   {
      aFeed_Divisor = 250000 / ((uint32_t)aFeed_mm * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) - 1;
      if (aFeed_Divisor < Start_Speed)
      {
         max_OCR4A = Start_Speed;
         if (Step_X_flag == OFF)
         {
            OCR4A = Start_Speed;
         }
      }
      else
      {
         max_OCR4A = aFeed_Divisor;
         OCR4A = aFeed_Divisor;
      }
   }
   else
   {
      aFeed_Divisor = Start_Speed;
      max_OCR4A = Start_Speed;
      OCR4A = Start_Speed;
   }

   Brake_Compens = (max_OCR4A - aFeed_Divisor) +1;
   Limit_Pos = Limit_Pos_Rear + Brake_Compens;

   Motor_X_Dir = CCW;
   Motor_X_CCW();
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   feed_X_flag = ON;
   Joy_X_flag = ON;
   Ena_INT_X_aFeed();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
void aFeed_Ext_Left()
{
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void aFeed_Int_Left()
{
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void aFeed_Ext_Right()
{
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void aFeed_Int_Right()
{
  
}
