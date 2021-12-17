//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** Print ***** /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Print()
{
   if (Mode == Mode_Thread)  //////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Thrd      %s", Thread_Info[Thread_Step].Thread_Print);
      
      if      (Sub_Mode_Thread == Sub_Mode_Thread_Int) snprintf(LCD_Row_2, 17, "Int  Max:%s", Thread_Info[Thread_Step].Limit_Print);
      else if (Sub_Mode_Thread == Sub_Mode_Thread_Man) snprintf(LCD_Row_2, 17, "Man  Max:%s", Thread_Info[Thread_Step].Limit_Print);
      else if (Sub_Mode_Thread == Sub_Mode_Thread_Ext) snprintf(LCD_Row_2, 17, "Ext  Max:%s", Thread_Info[Thread_Step].Limit_Print);
   } 

  
   else if (Mode == Mode_Feed)  //////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Feed mm/rev %1d.%02dmm", Feed_mm/100, Feed_mm%100);

      if      (Sub_Mode_Feed == Sub_Mode_Feed_Int) snprintf(LCD_Row_2, 17, "Int  Pq:%1d Ap:%1d.%02d", Pass_Total-Pass_Nr+1, Ap/100, Ap%100);
      else if (Sub_Mode_Feed == Sub_Mode_Feed_Man) snprintf(LCD_Row_2, 17, "Man  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_Feed == Sub_Mode_Feed_Ext) snprintf(LCD_Row_2, 17, "Ext  Pq:%1d Ap:%1d.%02d", Pass_Total-Pass_Nr+1, Ap/100, Ap%100);
   }


   else if (Mode == Mode_aFeed)  //////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Feed mm/min  %3d", aFeed_mm);
      
      if      (Sub_Mode_aFeed == Sub_Mode_aFeed_Int) snprintf(LCD_Row_2, 17, "Int  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_aFeed == Sub_Mode_aFeed_Man) snprintf(LCD_Row_2, 17, "Man  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_aFeed == Sub_Mode_aFeed_Ext) snprintf(LCD_Row_2, 17, "Ext  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
   }
 
   
   else if (Mode == Mode_Cone_L)  //////////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Cone < %s %1d.%02dmm", Cone_Info[Cone_Step].Cone_Print, Feed_mm/100, Feed_mm%100);

      if      (Sub_Mode_Cone == Sub_Mode_Cone_Int) snprintf(LCD_Row_2, 17, "Int  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_Cone == Sub_Mode_Cone_Man) snprintf(LCD_Row_2, 17, "Man  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_Cone == Sub_Mode_Cone_Ext) snprintf(LCD_Row_2, 17, "Ext  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
   }

   
   else if (Mode == Mode_Cone_R)  //////////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Cone > %s %1d.%02dmm", Cone_Info[Cone_Step].Cone_Print, Feed_mm/100, Feed_mm%100);
      
      if      (Sub_Mode_Cone == Sub_Mode_Cone_Int) snprintf(LCD_Row_2, 17, "Int  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_Cone == Sub_Mode_Cone_Man) snprintf(LCD_Row_2, 17, "Man  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
      else if (Sub_Mode_Cone == Sub_Mode_Cone_Ext) snprintf(LCD_Row_2, 17, "Ext  Pq:%1d Ap:%1d.%02d", Pass_Total, Ap/100, Ap%100);
   }


   else if (Mode == Mode_Reserve)  //////////////////////////////////////////////////////////////
   {
      snprintf(LCD_Row_1, 17, "Reserve         ");
      snprintf(LCD_Row_2, 17, "Reserve         ");
   }
   

   else if (Mode == Mode_Sphere)  //////////////////////////////////////////////////////////////
   {
      if (!key_sel_flag)
      {
         snprintf(LCD_Row_1, 17, "Sphr %2ld.%01ldmm %1d.%02dmm", Sph_R_mm * 2 / 100, Sph_R_mm * 2 / 10 %10, Feed_mm/100, Feed_mm%100);

         if      (Sub_Mode_Sphere == Sub_Mode_Sphere_Int) snprintf(LCD_Row_2, 17, "Mode not Exist  ");
         else if (Sub_Mode_Sphere == Sub_Mode_Sphere_Man) snprintf(LCD_Row_2, 17, "     BarDia %2ld.%01ld", Bar_R_mm*2/100, Bar_R_mm*2%100);
         else if (Sub_Mode_Sphere == Sub_Mode_Sphere_Ext) snprintf(LCD_Row_2, 17, "Ext  BarDia %2ld.%01ld", Bar_R_mm*2/100, Bar_R_mm*2%100);
      }
      
      else
      {
         snprintf(LCD_Row_1, 17, "Cut.Width %1d.%02dmm", Cutter_Width/100, Cutter_Width%100);
         snprintf(LCD_Row_2, 17, "Cut.StepZ %1d.%02dmm", Cutting_Width/100, Cutting_Width%100);
      }
   }

    
   else if (Mode == Mode_Divider)  /////////////////////////////////////////////////////////////
   { 
      long Spindle_Angle = Enc_Pos * 36000 / ENC_TICK;
      long Required_Angle = 36000 * (Current_Tooth - 1) / Total_Tooth;
      snprintf(LCD_Row_1, 17, "Req:%3ld.%02ld z:%3d", Required_Angle/100, Required_Angle%100, Total_Tooth);
      snprintf(LCD_Row_2, 17, "Rea:%3ld.%02ld a:%3d", Spindle_Angle/100, Spindle_Angle%100, Current_Tooth);
   }

   // Печать ошибки
   if      (err_1_flag == true) snprintf(LCD_Row_2, 17, "Limits not Set  ");
   else if (err_2_flag == true) snprintf(LCD_Row_2, 17, "Move to Init Pos");   
      
   lcd.setCursor(0, 0);
   lcd.print(LCD_Row_1);
   lcd.print("   ");

   lcd.setCursor(0, 1);
   lcd.print(LCD_Row_2);
   lcd.print("   ");
}
