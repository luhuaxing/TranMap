# ifndef DHEADER_H
# define DHEADER_H

# include "grdtypes.h"

#pragma warning(disable:4251)


class dHeader
{
	public:		
		//CONSTRUCTORS
		dHeader();
		dHeader( const dHeader & h );

		//Destructor
		~dHeader();

		//OPERATORS
		dHeader operator=( const dHeader & h );

		//DATA ACCESS MEMBERS
		long getNumberCols();
		long getNumberRows();
		double getNodataValue();
		double getDx();
		double getDy();
		double getXllcenter();
		double getYllcenter();
		char * getProjection();
		char * getNotes();

		void setNumberCols( long p_ncols );
		void setNumberRows( long p_nrows );
		void setNodataValue( double p_nodata_value );
		void setDx( double p_dx );
		void setDy( double p_dy );
		void setXllcenter( double p_xllcenter );
		void setYllcenter( double p_yllcenter );
		void setProjection( const char * p_projection );
		void setNotes( const char * p_notes );

	private:
		long ncols;
		long nrows;
		double nodataValue;
		double dx;
		double dy;
		double xllcenter;
		double yllcenter;
		CString projection;
		CString notes;
};

# endif