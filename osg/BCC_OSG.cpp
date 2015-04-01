/*  BCC_OSG.cpp

	This is the OSG Version, Written by John Bell July 2012
*/

#include <cstdlib>
#include <iostream>
//#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/UFOManipulator>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/NodeCallback>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/PolygonMode>


using namespace std;

const double Pi = acos( -1.0 );

// Function Prototypes

osg:: ref_ptr<osg::Node> createScene( void );
osg:: ref_ptr<osg::Node> createAxes( void );

/* Classic OSG event handlers do not work in omegalib. Omegalib functionality 
 * equivalent to this is implemented in the BCCApplication class below
// The Event Handler
class BCC_EventHandler : public osgGA::GUIEventHandler {

private:
	 osg::ref_ptr< osg::Node > node;
	 int wireframe;

public:
	BCC_EventHandler( osg::Node * n ) : node( n ), wireframe( 0 ) { }

	virtual bool handle ( const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa,
		osg::Object * obj, osg::NodeVisitor *nv ) {

		osg::PolygonMode::Mode mode[ 2 ] = { osg::PolygonMode::LINE, osg::PolygonMode::FILL };

		switch( ea.getEventType( ) ) {

			case osgGA::GUIEventAdapter::KEYDOWN:

				switch( ea.getKey( ) ) {

					case 'w':
					case 'W':
						osg::StateSet * set = node->getOrCreateStateSet( );
						osg::ref_ptr< osg::PolygonMode > pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, 
							mode[ wireframe ] );
						wireframe = !wireframe;
						set->setAttributeAndModes( pm.get( ), wireframe );
						return true;

				} // key

			default:
				return false;

		} // switch event type

		return false;

	} // handle
}; */

/* This is the old OpenSceneGraph main. Substituted by the omegalib main below
int main( void ) {

	cout << "Hello World\n";

	// Create a viewer, and attach a scene graph to it.

	osgViewer::Viewer viewer;
	osg::ref_ptr< osg::Node > root = createScene( ).get( );
	viewer.setSceneData( root.get( ) );
	viewer.addEventHandler( new BCC_EventHandler( root.get( ) ) );

	// Create a camera manipulator and attach it to the viewer main camera.

	osg::ref_ptr< osgGA::UFOManipulator > manipulator = new osgGA::UFOManipulator( );
	viewer.setCameraManipulator( manipulator  );

	// Set the manipulator home position and orientation.
	// The first point is the eye ( camera ) location,
	// The second is a point being looked at
	// The third is an "up" vector to uniquely determine a viewpoint

	manipulator->setHomePosition( osg::Vec3(1500,1500,500), osg::Vec3(0,600,300), osg::Vec3(0,0,1) );

	viewer.run( );

} */

// OMEGALIB STUFF BEGIN -------------------------------------------------------
#include <omega.h>
#include <omegaOsg/omegaOsg.h>

using namespace omega;
using namespace omegaOsg;

class BCCApplication: public EngineModule
{
public:
	void initialize()
	{
		// Create and setup the omegalib osg integration module
		myOsg = new OsgModule();
		ModuleServices::addModule(myOsg); 

		// Create an omegalib scene node and attach the root osg node to it. This is used to interact with the 
		// osg object through omegalib interactors.
		OsgSceneObject* oso = new OsgSceneObject(createScene().get());
		const AlignedBox3* bbox = oso->getBoundingBox();
		myOsg->setRootNode(oso->getTransformedNode());

		// Resize the entire scene
		myRoot = SceneNode::create("osgRoot");
		myRoot->addComponent(oso);
		myRoot->setScale(0.05f, 0.05f, 0.05f);
	}
private:
	Ref<OsgModule> myOsg;
	Ref<SceneNode> myRoot;
};

// Omegalib main
int main(int argc, char** argv) 
{
	Application<BCCApplication> app("bcc");
    return omain(app, argc, argv);
} // main
// OMEGALIB STUFF END -------------------------------------------------------

//---------------------------------------------------------------------------
// From this point on, code is the same as in the original OSG version.

// The cow rotater callback class
class cowRotater : public osg::NodeCallback {

	virtual void operator( ) ( osg::Node *node, osg::NodeVisitor *nv ) {

		// Check to verify that *node is a Matrix Transform, then typecast

		osg::MatrixTransform *mt = dynamic_cast< osg::MatrixTransform * > ( node );

		// Rotate the node.  Assume 60 fps

		osg::Matrixd m = mt->getMatrix( );
		osg::Matrixd r;
		double fps = 60.0;  // Assumed 60 fps.  Change if measurable.
		//double revPerSec = 1.0 * 2.0 * Pi / fps;  // First number is revolutions per second.
		double revPerSec = 0.25 * 360.0 / fps;  // First number is revolutions per second.
		r.makeRotate( revPerSec / 60.0, osg::Vec3f( 0.0, 0.0, 1.0 ) );
		mt->setMatrix( r * m );

		

		// Call the parent function traverse to visit the children

		traverse ( node, nv);

	} // cowRotater::operator( ) method


};

osg:: ref_ptr<osg::Node> createScene( void ) {

	cout << "Building\n";

	// Building from the top down - Start with the root

	osg::ref_ptr< osg::Group > root = new osg::Group;

	// Declare a single matrix, which will be re-used repeatedly for each Transform

	osg::Matrix m;
	osg::ref_ptr< osg::MatrixTransform > mt = NULL;
	
	// Now to create red and pink Geodes, to hang on the MatrixTransforms

	osg::ref_ptr< osg::Sphere > ball = new osg::Sphere( osg::Vec3f( 0.0, 0.0, 0.0 ), 10.0 );

	osg::ref_ptr< osg::Geode > redGeode = new osg::Geode;
	osg::ref_ptr< osg::ShapeDrawable > redBall = new osg::ShapeDrawable( ball );
	redBall->setColor( osg::Vec4( 1.0, 0.0, 0.0, 1.0 ) );
	redGeode->addDrawable( redBall );

	osg::ref_ptr< osg::Geode > pinkGeode = new osg::Geode;
	osg::ref_ptr< osg::ShapeDrawable > pinkBall = new osg::ShapeDrawable( ball );
	pinkBall->setColor( osg::Vec4( 1.0, 0.5, 0.5, 1.0 ) );
	pinkGeode->addDrawable( pinkBall );


	// Now to build a whole array of spheres

	for( int ix = 0; ix < 4; ix++ ) {
		for( int iy = 0; iy < 4; iy++ ) {
			for( int iz = 0; iz < 4; iz++ ) { 

			// Create the  matrixTransform, set its value, and add it

			mt = new osg::MatrixTransform;
			m.setTrans( 100 * ix, 100 * iy, 100 * iz );
			mt->setMatrix( m );
			root->addChild( mt.get( ) );

			if( ix < 1 || ix > 2 || iy < 1 || iy > 2 || iz < 1 || iz > 2 )
				mt->addChild( pinkGeode );
			else
				mt->addChild( redGeode );

			} // iz
		} // iy
	} // ix

	// Now to build another one in between the first one ( BCC )

	for( int ix = 0; ix < 3; ix++ ) {
		for( int iy = 0; iy < 3; iy++ ) {
			for( int iz = 0; iz < 3; iz++ ) { 

			// Create the  matrixTransform, set its value, and add it

			mt = new osg::MatrixTransform;
			m.setTrans( 100 * ix + 50, 100 * iy + 50, 100 * iz + 50 );
			mt->setMatrix( m );
			root->addChild( mt.get( ) );

			if( ix == 1 && iy == 1 && iz == 1 )
				mt->addChild( redGeode );
			else
				mt->addChild( pinkGeode );

			} // iz
		} // iy
	} // ix

	root->addChild( createAxes( ).get( ) );

	osg::ref_ptr< osg::Node > cow = osgDB::readNodeFile( "cow.osg" );
	mt = new osg::MatrixTransform;
	m.makeScale( 10.0, 10.0, 10.0 );
	mt->setMatrix( m );
	mt->addChild( cow.get( ) );
	mt->addUpdateCallback( new cowRotater( ) );
	root->addChild( mt.get( ) );

	// Now to add a semitransparent box

	osg::ref_ptr< osg::Box > box = new osg::Box( osg::Vec3f( 150.0, 150.0, 150.0 ), 100.0 );
	osg::ref_ptr< osg::Geode > boxGeode = new osg::Geode;
	osg::ref_ptr< osg::ShapeDrawable > blueBox = new osg::ShapeDrawable( box );
	blueBox->setColor( osg::Vec4( 0.5, 0.5, 1.0, 0.5 ) );
	boxGeode->addDrawable( blueBox );
	// Obtain the StateSet.
	osg::StateSet* stateSet = boxGeode->getOrCreateStateSet();
	// Enable blending in this StateSet.
	stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
	root->addChild( boxGeode.get( ) );


	// The following lines were copied from the quick start guide

	if (!root.valid())
		osg::notify(osg::FATAL) <<
			"Failed in createSceneGraph()." << endl;
	bool result = osgDB::writeNodeFile(
		*(root.get()), "BCC.osg" );
	if ( !result )
		osg::notify(osg::FATAL) <<
		"Failed in osgDB::writeNodeFile()." << endl;


	return root.get( );
	

} // createScene

osg:: ref_ptr<osg::Node> createAxes( void ) {

	// This method should be made more succinct by using arrays.
	// It should also be generalized to create unit axes, that can then be 
	// scaled and translated, etc. as needed.

	const osg::Vec4f red  ( 1.0, 0.0, 0.0, 1.0 ), 
			 green( 0.0, 1.0, 0.0, 1.0 ), 
			 blue ( 0.0, 0.0, 1.0, 1.0 );
	osg::Matrix mR, mT;

	osg::ref_ptr< osg::Group > root = new osg::Group;
	osg::ref_ptr< osg::MatrixTransform > mt = NULL;
	osg::ref_ptr< osg::Geode > geode = NULL;

	osg::ref_ptr< osg::Cylinder > axis = new osg::Cylinder( osg::Vec3f( 0.0, 0.0, 0.0 ), 2.0, 500.0 );
	osg::ref_ptr< osg::Cone > arrow = new osg::Cone( osg::Vec3f( 0.0, 0.0, 0.0 ), 10.0, 20.0 );

	// Draw the X axis in Red

	osg::ref_ptr< osg::ShapeDrawable > xAxis = new osg::ShapeDrawable( axis );
	xAxis->setColor( red );
	geode = new osg::Geode;
	geode->addDrawable( xAxis.get( ) );
	mT.makeTranslate( 250.0, 0.0, 0.0 );
	mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix( mR * mT );
	mt->addChild( geode.get( ) );
	root->addChild( mt.get( ) );

	osg::ref_ptr< osg::ShapeDrawable > xArrow= new osg::ShapeDrawable( arrow );
	xArrow->setColor( red );
	geode = new osg::Geode;
	geode->addDrawable( xArrow.get( ) );
	mT.makeTranslate( 500.0, 0.0, 0.0 );
	mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix( mR * mT );
	root->addChild( mt.get( ) );
	mt->addChild( geode.get( ) );

	// Then the Y axis in green

	osg::ref_ptr< osg::ShapeDrawable > yAxis = new osg::ShapeDrawable( axis );
	yAxis->setColor( green );
	geode = new osg::Geode;
	geode->addDrawable( yAxis.get( ) );
	mT.makeTranslate( 0.0, 250.0, 0.0 );
	mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( -1.0, 0.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix( mR * mT );
	mt->addChild( geode.get( ) );
	root->addChild( mt.get( ) );

	osg::ref_ptr< osg::ShapeDrawable > yArrow= new osg::ShapeDrawable( arrow );
	yArrow->setColor( green );
	geode = new osg::Geode;
	geode->addDrawable( yArrow.get( ) );
	mT.makeTranslate( 0.0, 500.0, 0.0 );
	mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( -1.0, 0.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix( mR * mT );
	root->addChild( mt.get( ) );
	mt->addChild( geode.get( ) );

	// And the Z axis in blue

	osg::ref_ptr< osg::ShapeDrawable > zAxis = new osg::ShapeDrawable( axis );
	zAxis->setColor( blue );
	geode = new osg::Geode;
	geode->addDrawable( zAxis.get( ) );
	mT.makeTranslate( 0.0, 0.0, 250.0 );
	//mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix( mT );
	mt->addChild( geode.get( ) );
	root->addChild( mt.get( ) );

	osg::ref_ptr< osg::ShapeDrawable > zArrow= new osg::ShapeDrawable( arrow );
	zArrow->setColor( blue );
	geode = new osg::Geode;
	geode->addDrawable( zArrow.get( ) );
	mT.makeTranslate( 0.0, 0.0, 500.0 );
	//mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
	mt = new osg::MatrixTransform;
	mt->setMatrix(mT );
	root->addChild( mt.get( ) );
	mt->addChild( geode.get( ) );

	return root.get( );

} // createAxes

