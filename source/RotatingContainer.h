#ifndef _ROTATINGCONTAINER_
#define _ROTATINGCONTAINER_

#include "oxygine_include.h"
#include "Actor.h"
#include "math/vector2.h"
#include "Draggable.h"
#include "ClipRectActor.h"

namespace oxygine
{
	DECLARE_SMART(RotatingContainer, spRotatingContainer);
	class RotatingContainer : public Actor
	{
	public:
		static void setDefaultTouchThreshold(float val);

		class RotatingEvent: public Event
		{
		public:
			enum EV {
				BEGIN = makefourcc('R', 'C', 'B', 'G'),
				SLIDING = makefourcc('R', 'C', 'S', 'L'),
				END = makefourcc('R', 'C', 'E', 'D')
			};

			RotatingEvent(EV ev):Event(ev), speed(0, 0) {}
			Vector2 speed;
		};

		RotatingContainer(void);
		~RotatingContainer(void);

		spActor getContent() const { return _content; }
		const RectF& getDragBounds() const { return _drag.getDragBounds(); }

		/**max allowed radius of touch move when content could be clicked*/
		void setTouchThreshold(float rad);
		void setContent(spActor content);
		void setLocked(bool locked);
		void snap();

	protected:
		void destroy();

		void handleEvent(Event *event);

		void sizeChanged(const Vector2 &size);
		void doUpdate(const UpdateState &us);
		void updateDragBounds();
		

		void _newEvent(Event *event);

		bool _sliding;
		float _rad;
		float _maxSpeed;
		timeMS _downTime;

		Vector2 _downPos;
		Vector2 _speed;

		Draggable _drag;

		spActor _content;
		spClipRectActor _clip;

		spEventDispatcher _holded;

		struct  iter {
			Vector2 pos;
			timeMS tm;
		};

		timeMS _lastTime;
		enum { NUM = 11 };
		iter _prev[NUM];
		int _current;

		timeMS _lastIterTime;
	};

	/*class Element {
		public:
			spActor actor;
			float z;
			float positionDegrees;
			
			Element();
			~Element();
			//public function Element(child:DisplayObject, positionDegrees:Number) {
			//	this.child = child;
			//	this.positionDegrees = positionDegrees;
			//}
	};*/

}

#endif
/*
	public class RotatingContainer extends DisplayObject 
	{
		private var _width:Number;
		private var _height:Number;
		private var R:Number;
		private var SCALE:Number;
		private var FRONT_CAPACITY:int;
		private var _elements:Vector.<Element>;
		private var SEPARATION_DEGREES:Number;
		private var _offsetDegrees:Number;
		private var _previousOffsetDegrees:Number;
		private var _context:Context;
		private var _touchQuad:Quad;
		private var _isHolding:Boolean;
		private var _currentIndex:int;
		private var _touchBeginTime:int;
		
		public function RotatingContainer(c:Context, width:Number, height:Number, r:Number, scale:Number, frontCapacity:int, elements:Vector.<DisplayObject>, currentIndex:int) {
			_context = c;
			_width = width;
			_height = height;
			R = r;
			SCALE = scale;
			FRONT_CAPACITY = frontCapacity;
			_elements = new Vector.<Element>();
			_previousOffsetDegrees = 0.0;
			_offsetDegrees = 0.0;
			_currentIndex = currentIndex;
			addElements(elements);
			draw();
			
			_touchQuad = new Quad(_context.engine.materialManager.alphaMaterial, _width, _height, 0x000000);
			_touchQuad.alpha = 0.0;
			addChildAt(_touchQuad, 0);
			addChild(_touchQuad);
			addEventListener(TouchEvent.TOUCH_DOWN, onTouchDown);
			addEventListener(TouchEvent.PAN, onPan);
			addEventListener(TouchEvent.TOUCH_UP, onTouchUp);
			addEventListener(TouchEvent.MOVED_OUT, onMovedOut);
		}
		
		public function getCurrentChild():DisplayObject {
			return _elements[currentIndex].child;
		}
		
		private function addElements(elements:Vector.<DisplayObject>):void {
			SEPARATION_DEGREES = 360 / elements.length;
			for (var i:int = 0; i < elements.length; ++i) {
				var child:DisplayObject = elements[i];
				if (child is WindowWithGlow) {
					if (i == _currentIndex) {
						(child as WindowWithGlow).enableGlow();
					} else {
						(child as WindowWithGlow).disableGlow();
					}
				}
				child.addEventListener(TouchEvent.TAP, onChildTapped);
				_elements.push(new Element(child, (i - _currentIndex) * SEPARATION_DEGREES));
			}
		}
		
		private function onChildTapped(e:TouchEvent):void {
			rotateToGivenDegrees(-getChildDegrees(e.target as DisplayObject));
		}
		
		private function getChildDegrees(child:DisplayObject):Number {
			for each (var elem:Element in _elements) {
				if (elem.child == child) {
					return elem.positionDegrees;
				}
			}
			return _offsetDegrees;
		}
		
		public function tick(deltaTime:Number):void {
			draw();
			var diff:int = Math.floor((_previousOffsetDegrees - SEPARATION_DEGREES / 2) / SEPARATION_DEGREES) - Math.floor((_offsetDegrees - SEPARATION_DEGREES / 2) / SEPARATION_DEGREES);
			if (diff > 0) {
				for (var i:int = 0; i < diff; ++i) {
					if (_elements[_currentIndex].child is WindowWithGlow) {
						(_elements[_currentIndex].child as WindowWithGlow).disableGlow();
					}
					++_currentIndex;
					_currentIndex %= _elements.length;
					if (_elements[_currentIndex].child is WindowWithGlow) {
						(_elements[_currentIndex].child as WindowWithGlow).enableGlow();
					}
					dispatchEvent(new Event(Event.CHANGED));
				}
			} else if (diff < 0) {
				for (i = 0; i > diff; --i) {
					if (_elements[_currentIndex].child is WindowWithGlow) {
						(_elements[_currentIndex].child as WindowWithGlow).disableGlow();
					}
					--_currentIndex;
					_currentIndex = (_currentIndex + _elements.length) % _elements.length;
					if (_elements[_currentIndex].child is WindowWithGlow) {
						(_elements[_currentIndex].child as WindowWithGlow).enableGlow();
					}
					dispatchEvent(new Event(Event.CHANGED));
				}
			}
			_previousOffsetDegrees = _offsetDegrees;
		}
		
		private function draw():void {
			for (var i:int = _currentIndex - int(FRONT_CAPACITY / 2); i <= _currentIndex + int(FRONT_CAPACITY / 2); ++i) {
				var wrappedI:int = (i + _elements.length) % _elements.length;
				var element:Element =  _elements[wrappedI];
				var deltaX:Number = R * Math.sin(Utils.degToRad(element.positionDegrees + _offsetDegrees));
				var deltaZ:Number = R * (1 - Math.cos(Utils.degToRad(element.positionDegrees + _offsetDegrees)));
				var scale:Number = Math.max(0, 1 - (1 - SCALE) / (2 * R) * deltaZ);
				var child:DisplayObject = element.child;
				child.x = middleX + deltaX - child.width * scale / 2;
				child.y = middleY - child.height * scale / 2;
				child.scale = scale;
				if (child is WindowWithGlow) {
					var m:MaterialColorModifier = (child as WindowWithGlow).material as MaterialColorModifier;
					m.r = m.g = m.b = (scale - 1) * 1.0;
				}
				_elements[wrappedI].z = deltaZ;
			}
			drawSortedByZ();
		}
		
		private function drawSortedByZ():void {
			var leftIdx:int = (_currentIndex - int(FRONT_CAPACITY / 2) + _elements.length) % _elements.length;
			var rightIdx:int = (_currentIndex + int(FRONT_CAPACITY / 2)) % _elements.length;
			if (leftIdx < rightIdx) {
				var elements:Vector.<Element> = _elements.slice(leftIdx, rightIdx + 1);
			} else {
				elements = _elements.slice(0, rightIdx + 1);
				elements = elements.concat(_elements.slice(leftIdx));
			}
			elements.sort(function (x:Element, y:Element):Number { if (x.z > y.z) return -1; return 1; } );
			removeChildren();
			for each (var elem:Element in elements) {
				addChild(elem.child);
			}
			if (_touchQuad) {
				addChildAt(_touchQuad, 0);
			}
		}
		
		private function get middleX():Number {
			return _width / 2;
		}
		
		private function get middleY():Number {
			return _height / 2;
		}
		
		override public function get width():Number {
			return _width;
		}
		
		override public function get height():Number {
			return _height;
		}
		
		public function get offsetDegrees():Number {
			return _offsetDegrees;
		}
		
		public function set offsetDegrees(value:Number):void {
			_offsetDegrees = value;
		}
		
		public function get currentIndex():int {
			return _currentIndex;
		}
		
		public function get elements():Vector.<DisplayObject> {
			var objects:Vector.<DisplayObject> = new Vector.<DisplayObject>();
			for each(var e:Element in _elements) {
				objects.push(e.child);
			}
			return objects;
		}
		
		private function onTouchDown(e:TouchEvent):void {
			e.stopImmediatePropagation();
			_isHolding = true;
			_touchBeginTime = getTimer();
		}
		private function onPan(e:PanEvent):void {
			e.stopImmediatePropagation();
			if (_isHolding) {
				_offsetDegrees += e.deltaX / (240 / SEPARATION_DEGREES);
			}
		}
		
		private function onTouchUp(e:TouchEvent):void {
			e.stopImmediatePropagation();
			onTouchEnded();
		}
		
		private function onMovedOut(e:TouchEvent):void {
			if (!Utils.inInterval(e.globalX, x, x + width) || !Utils.inInterval(e.globalY, y, y + height)) {
				onTouchEnded();
			}
		}
		
		private function onTouchEnded():void {
			_isHolding = false;
			var destDegrees:Number = 0;
			if (Utils.inInterval(_offsetDegrees % SEPARATION_DEGREES, 0, SEPARATION_DEGREES / 2)
				||
				Utils.inInterval(_offsetDegrees % SEPARATION_DEGREES, -SEPARATION_DEGREES, -SEPARATION_DEGREES / 2)) {
				//go left
				destDegrees = Math.floor(_offsetDegrees / SEPARATION_DEGREES) * SEPARATION_DEGREES;
			} else { //go right
				destDegrees = Math.ceil(_offsetDegrees / SEPARATION_DEGREES) * SEPARATION_DEGREES;
			}
			rotateToGivenDegrees(destDegrees);
		}
		
		private function rotateToGivenDegrees(degrees:Number):void {
			offsetDegrees = (offsetDegrees + 360) % 360;
			degrees = (degrees + 360) % 360;
			var minusDegrees:Number = degrees <= 0 ? degrees : degrees - 360.0;
			var plusDegrees:Number = minusDegrees + 360.0;
			var t:Tween = new Tween(this, 0.3, Transitions.EASE_OUT);
			_context.engine.removeTweens(this);
			_context.engine.addTween(t);
			if (Math.abs(offsetDegrees - minusDegrees) < Math.abs(offsetDegrees - plusDegrees)) {
				t.animate("offsetDegrees", minusDegrees);
			} else {
				t.animate("offsetDegrees", plusDegrees);
			}
		}
	}
}
import badass.engine.DisplayObject;


*/