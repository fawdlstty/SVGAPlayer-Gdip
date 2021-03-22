#include "SvgaPrivate.h"



std::vector<std::string> __split_comma_space (const std::string &_spath) {
	std::vector<std::string> _v;
	std::string _tmp = "";
	for (int i = 0; i < _spath.size (); ++i) {
		char _ch = _spath [i];
		if (_ch == ',' || _ch == ' ' || _ch == '\t' || _ch == '\r' || _ch == '\n') {
			if (_tmp != "") {
				_v.push_back (_tmp);
				_tmp = "";
			}
		} else {
			_tmp += _ch;
		}
	}
	if (_tmp != "") {
		_v.push_back (_tmp);
		_tmp = "";
	}
	return _v;
}

namespace SvgaLib {
	class SvgaClipPath_t {
	public:
		SvgaClipPath_t () = default;

		bool Parse (const std::string &_spath) {
			if (_spath == "")
				return true;
			m_spath = _spath;
			std::vector<std::string> _v = __split_comma_space (m_spath);
			//
			std::function<char ()> _get_ch;
			_get_ch = [&_get_ch, &_v] () -> char {
				if (_v.empty ())
					return 0;
				if (_v [0].empty ()) {
					_v.erase (_v.begin ());
					return _get_ch ();
				}
				char ch = _v [0][0];
				if (_v [0].size () == 1) {
					_v.erase (_v.begin ());
				} else {
					_v [0].erase (_v [0].begin ());
				}
				return ch;
			};
			std::function<float ()> _get_float = [&_v] () -> float {
				if (_v.empty ())
					return 0.0f;
				float _f = std::stof (_v [0]);
				_v.erase (_v.begin ());
				return _f;
			};
			std::function<Gdiplus::PointF ()> _get_floats = [&_v] () -> Gdiplus::PointF {
				Gdiplus::PointF _f;
				if (!_v.empty ()) {
					_f.X = std::stof (_v [0]);
					_v.erase (_v.begin ());
				}
				if (!_v.empty ()) {
					_f.Y = std::stof (_v [0]);
					_v.erase (_v.begin ());
				}
				return _f;
			};
			//
			bool _init = true;
			Gdiplus::PointF _p_start { 0, 0 };
			Gdiplus::PointF _p_last { 0, 0 };
			Gdiplus::PointF _ps [4] { {0, 0}, {0, 0}, {0, 0}, {0, 0} };
			Gdiplus::PointF _p { 0, 0 };
			while (!_v.empty ()) {
				switch (_get_ch ()) {
				case 'M':
					_p = _get_floats ();
					if (_init) {
						_init = false;
						_p_start = _p;
					}
					//m_path.moveTo (posX, posY);
					break;
				case 'm':
					_p.X = _p_last.X + _get_float ();
					_p.Y = _p_last.Y + _get_float ();
					if (_init) {
						_init = false;
						_p_start = _p;
					}
					//m_path.moveTo (posX, posY);
					break;
				case 'L':
					_p = _get_floats ();
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'l':
					_p.X = _p_last.X + _get_float ();
					_p.Y = _p_last.Y + _get_float ();
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'H':
					_p.X = _get_float ();
					_p.Y = _p_last.Y;
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'h':
					_p.X = _p_last.X + _get_float ();
					_p.Y = _p_last.Y;
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'V':
					_p.X = _p_last.X;
					_p.Y = _get_float ();
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'v':
					_p.X = _p_last.X;
					_p.Y = _p_last.Y + _get_float ();
					m_path.AddLine (_p_last, _p);
					//m_path.lineTo (posX, posY);
					break;
				case 'C':
					_ps [0] = _p_last;
					_ps [1] = _get_floats ();
					_ps [2] = _get_floats ();
					_ps [3] = _get_floats ();
					_p = _ps [3];
					//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
					m_path.AddCurve (_ps, 4);
					break;
				case 'c':
					_ps [0] = _p_last;
					_ps [1] = _get_floats ();
					_ps [2] = _get_floats ();
					_ps [3].X = _p_last.X + _get_float ();
					_ps [3].Y = _p_last.Y + _get_float ();
					_p = _ps [3];
					//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
					m_path.AddCurve (_ps, 4);
					break;
				case 'S':
					if (_ps [1].X && _ps [1].Y && _ps [2].X && _ps [2].Y) {
						_ps [0] = _p_last;
						_ps [1].X = _p_last.X * 2 - _ps [2].X;
						_ps [1].Y = _p_last.Y * 2 - _ps [2].Y;
						_ps [2] = _get_floats ();
						_ps [3] = _get_floats ();
						_p = _ps [2];
						m_path.AddCurve (_ps, 4);
						//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
					} else {
						_ps [0] = _p_last;
						_ps [1] = _get_floats ();
						_ps [2] = _get_floats ();
						_p = _ps [2];
						m_path.AddCurve (_ps, 3);
						//m_path.quadTo (posX1, posY1, posX, posY);
					}
					break;
				case 's':
					if (_ps [1].X && _ps [1].Y && _ps [2].X && _ps [2].Y) {
						_ps [0] = _p_last;
						_ps [1].X = _p_last.X * 2 - _ps [2].X;
						_ps [1].Y = _p_last.Y * 2 - _ps [2].Y;
						_ps [2] = _get_floats ();
						_ps [3].X = _p_last.X + _get_float ();
						_ps [3].Y = _p_last.Y + _get_float ();
						_p = _ps [2];
						m_path.AddCurve (_ps, 4);
						//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
					} else {
						_ps [0] = _p_last;
						_ps [1] = _get_floats ();
						_ps [2].X = _p_last.X + _get_float ();
						_ps [2].Y = _p_last.Y + _get_float ();
						_p = _ps [2];
						m_path.AddCurve (_ps, 3);
						//m_path.quadTo (posX1, posY1, posX, posY);
					}
					break;
				case 'Q':
					_ps [0] = _p_last;
					_ps [1] = _get_floats ();
					_ps [2] = _get_floats ();
					_p = _ps [2];
					m_path.AddCurve (_ps, 3);
					//m_path.quadTo (posX1, posY1, posX, posY);
					break;
				case 'q':
					_ps [0] = _p_last;
					_ps [1] = _get_floats ();
					_ps [2].X = _p_last.X + _get_float ();
					_ps [2].Y = _p_last.Y + _get_float ();
					_p = _ps [2];
					m_path.AddCurve (_ps, 3);
					//m_path.quadTo (posX1, posY1, posX, posY);
					break;
				case 'A':
					break;
				case 'a':
					break;
				case 'Z':
				case 'z':
					m_path.AddLine (_p_last, _p_start);
					//m_path.closeSubpath ();
					break;
				default:
					break;
				}
				_p_last = _p;
			}
			return true;
		}

		std::tuple<ResType_t, Image_t *> Clip (Image_t *_src_img) {
			if (m_spath.empty ())
				return { ResType_t::Ignore, _src_img };
			Image_t *_img = ImageEngine_t::CreateImage ((int32_t) _src_img->GetWidth (), (int32_t) _src_img->GetHeight (), true);
			Gdiplus::Graphics _g (_img);
			_g.SetSmoothingMode (Gdiplus::SmoothingModeAntiAlias);
			_g.SetClip (&m_path);
			Gdiplus::Rect _r { 0, 0, (INT) _src_img->GetWidth (), (INT) _src_img->GetHeight () };
			_g.DrawImage (_src_img, _r, 0, 0, _r.Width, _r.Height, Gdiplus::UnitPixel);
			return { ResType_t::NeedRelease, _img };
		}

	private:
		std::string m_spath = "";
		Gdiplus::GraphicsPath m_path;
	};



	class SvgaVideoSpriteFrameImpl_t: public ISvgaVideoSpriteFrame_t {
	public:
		SvgaVideoSpriteFrameImpl_t () = default;
		virtual ~SvgaVideoSpriteFrameImpl_t () = default;
		std::tuple<ResType_t, Image_t *> Clip (Image_t *_src_img) override { return m_clip_path.Clip (_src_img); }
		float GetAlpha () override { return m_alpha; }
		Transform_t GetTransform () override { return m_transform; }

		float m_alpha = 0.0f;
		RectF_t m_layout;
		Transform_t m_transform;
		SvgaClipPath_t m_clip_path;
	};



	std::shared_ptr<ISvgaVideoSpriteFrame_t> __parse_SvgaVideoSpriteFrame_t_v2 (const com::opensource::svga::FrameEntity &_fe) {
		auto _ptr = std::make_shared<SvgaVideoSpriteFrameImpl_t> ();
		_ptr->m_alpha = _fe.alpha ();
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
		if (_fe.has_layout ()) {
			x = _fe.layout ().x ();
			y = _fe.layout ().y ();
			w = _fe.layout ().width ();
			h = _fe.layout ().height ();
		}
		_ptr->m_layout = RectF_t (x, y, w, h);
		//
		float a = 1.0f;
		float b = 0.0f;
		float c = 0.0f;
		float d = 1.0f;
		float tx = 0.0f;
		float ty = 0.0f;
		if (_fe.has_transform ()) {
			a = _fe.transform ().a ();
			b = _fe.transform ().b ();
			c = _fe.transform ().c ();
			d = _fe.transform ().d ();
			tx = _fe.transform ().tx ();
			ty = _fe.transform ().ty ();
		}
		_ptr->m_transform = Transform_t (a, b, c, d, tx, ty);
		//
		if (!_ptr->m_clip_path.Parse (_fe.clippath ()))
			return nullptr;
		return _ptr;
	}
}
