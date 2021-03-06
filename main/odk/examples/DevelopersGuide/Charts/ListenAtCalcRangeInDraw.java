/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// __________ Imports __________

// base classes
import com.sun.star.uno.XInterface;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.lang.*;

// property access
import com.sun.star.beans.*;

// application specific classes
import com.sun.star.chart.*;
import com.sun.star.drawing.*;

import com.sun.star.table.CellRangeAddress;
import com.sun.star.table.XCellRange;
import com.sun.star.sheet.XSpreadsheetDocument;

import com.sun.star.frame.XModel;
import com.sun.star.frame.XController;

import com.sun.star.util.XNumberFormatsSupplier;
import com.sun.star.util.XNumberFormats;

// base graphics things
import com.sun.star.awt.Point;
import com.sun.star.awt.Size;
import com.sun.star.awt.Rectangle;
import com.sun.star.awt.FontWeight;
import com.sun.star.awt.FontRelief;

// Exceptions
import com.sun.star.uno.Exception;
import com.sun.star.uno.RuntimeException;
import com.sun.star.beans.UnknownPropertyException;
import com.sun.star.lang.IndexOutOfBoundsException;
import com.sun.star.util.MalformedNumberFormatException;


// __________ Implementation __________

/** Create a spreadsheet add some data.
    Create a presentation and add a chart.
    Connect the chart to a calc range via a listener
    @author Bj&ouml;rn Milcke
 */
public class ListenAtCalcRangeInDraw implements XChartDataChangeEventListener
{
    public static void main( String args[] )
    {
        ListenAtCalcRangeInDraw aMySelf = new ListenAtCalcRangeInDraw( args );

        aMySelf.run();
    }

    public ListenAtCalcRangeInDraw( String args[] )
    {
        Helper aHelper = new Helper( args );

        maSheetDoc = aHelper.createSpreadsheetDocument();
        maDrawDoc  = aHelper.createDrawingDocument();
        CalcHelper aCalcHelper   = new CalcHelper(  maSheetDoc );
        ChartHelper aChartHelper = new ChartHelper( maDrawDoc );

        XCellRange aRange = aCalcHelper.insertFormulaRange( 3, 30 );

        // the unit for measures is 1/100th of a millimeter
        // position at (1cm, 1cm)
        Point aPos    = new Point( 1000, 1000 );

        // size of the chart is 15cm x 9.271cm
        Size  aExtent = new Size( 15000, 9271 );

        // insert a new chart into the "Chart" sheet of the
        // spreadsheet document
        maChartDocument = aChartHelper.insertOLEChartInDraw(
            "ChartWithCalcData",
            aPos,
            aExtent,
            "com.sun.star.chart.XYDiagram" );

        // attach the data coming from the cell range to the chart
        maChartData = (XChartData) UnoRuntime.queryInterface( XChartData.class, aRange );
        maChartDocument.attachData( maChartData );
    }

    // ____________________

    public void run()
    {        
        try
        {
            ((XPropertySet) UnoRuntime.queryInterface(
                XPropertySet.class, maChartDocument )).setPropertyValue(
                "HasSubTitle", new Boolean( true ));

            // start listening for death of spreadsheet
            ((XComponent) UnoRuntime.queryInterface(
                XComponent.class, maSheetDoc )).addEventListener( this );

            // start listening for death of chart
            ((XComponent) UnoRuntime.queryInterface(
                XComponent.class, maChartDocument )).addEventListener( this );

            //start listening for change of data
            maChartData.addChartDataChangeEventListener( this );
        }
        catch( Exception ex )
        {
            System.out.println( "Oops: " + ex );
        }

        // call listener
        ChartDataChangeEvent aEvent = new ChartDataChangeEvent();
        aEvent.Type = ChartDataChangeType.ALL;
        chartDataChanged( aEvent );
    }

    // ____________________

    // XEventListener (base of XChartDataChangeEventListener)
    public void disposing( EventObject aSourceObj )
    {
        if( UnoRuntime.queryInterface( XChartDocument.class, aSourceObj.Source ) != null )
            System.out.println( "Disconnecting Listener because Chart was shut down" );

        if( UnoRuntime.queryInterface( XSpreadsheetDocument.class, aSourceObj.Source ) != null )
            System.out.println( "Disconnecting Listener because Spreadsheet was shut down" );

        // remove data change listener
        maChartData.removeChartDataChangeEventListener( this );

        // remove dispose listeners
        ((XComponent) UnoRuntime.queryInterface(
            XComponent.class, maSheetDoc )).removeEventListener( this );
        ((XComponent) UnoRuntime.queryInterface(
            XComponent.class, maChartDocument )).removeEventListener( this );

        System.exit( 0 );
    }

    // ____________________

    // XChartDataChangeEventListener
	public void chartDataChanged( ChartDataChangeEvent aEvent )
    {
        // update subtitle
        String aTitle = new String( "Last Update: " + new java.util.Date( System.currentTimeMillis() ));

        try
        {
            XPropertySet aDocProp = (XPropertySet) UnoRuntime.queryInterface(
                XPropertySet.class, maChartDocument );
            aDocProp.setPropertyValue( "HasMainTitle", new Boolean( true ));
            
            ((XPropertySet) UnoRuntime.queryInterface(
                XPropertySet.class, maChartDocument.getSubTitle())).setPropertyValue(
                "String", aTitle );

            maChartDocument.attachData( maChartData );
        }
        catch( Exception ex )
        {
            System.out.println( "Oops: " + ex );
        }

        System.out.println( "Data has changed" );
    }


    // __________ private __________

    private XSpreadsheetDocument      maSheetDoc;
    private XModel                    maDrawDoc;
    private XChartDocument            maChartDocument;
    private XChartData                maChartData;
}
