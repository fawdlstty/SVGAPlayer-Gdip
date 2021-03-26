#include "SvgaPrivate.h"



static std::vector<std::string> __split_comma_and_space (const std::string &_spath) {
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

static float pow2 (const float f) { return f * f; }

static float _get_angle (const PointF_t &a, const PointF_t &b) {
	float prod = a.X * b.X + a.Y * b.Y;
	float div = sqrt ((pow2 (a.X) + pow2 (a.Y)) * (pow2 (b.X) + pow2 (b.Y)));
	float sign = (a.X * b.Y - a.Y * b.X) > 0.0f ? 1.0f : -1.0f;
	return acos (prod / div) * sign * 180.0f / (float) 3.1415927f;
}

namespace SvgaLib {
	class SvgaClipPath_t {
	public:
		SvgaClipPath_t () {
			m_path.SetFillMode (Gdiplus::FillModeAlternate);
		}

		static std::unique_ptr<SvgaClipPath_t> Parse (const std::string &_spath) {
			if (_spath.empty ())
				return nullptr;
			// https://github.com/Claybird/suzume/blob/master/suzume.cpp
			std::vector<std::string> _v = __split_comma_and_space (_spath);
			std::function<char ()> _get_ch;
			char _last_ch = '\0';
			_get_ch = [&] () -> char {
				if (_v.empty ()) return 0;
				if (_v [0].empty ()) { _v.erase (_v.begin ()); return _get_ch (); }
				char ch = _v [0][0];
				if (_v [0].size () == 1) { _v.erase (_v.begin ()); } else { _v [0].erase (_v [0].begin ()); }
				if (ch < 'A' || (ch > 'Z' && ch < 'a') || ch > 'z')
					ch = (_last_ch >= 'a' && _last_ch <= 'z') ? 'l' : 'L';
				_last_ch = ch;
				return ch;
			};
			std::function<int ()> _get_i = [&] () -> float {
				if (_v.empty ())
					return 0;
				int _i = std::stoi (_v [0]);
				_v.erase (_v.begin ());
				return _i;
			};
			std::function<float ()> _get_f = [&] () -> float {
				if (_v.empty ())
					return 0.0f;
				float _f = std::stof (_v [0]);
				_v.erase (_v.begin ());
				return _f;
			};
			std::function<PointF_t ()> _get_pt = [&] () -> PointF_t {
				PointF_t _f;
				_f.X = _get_f ();
				_f.Y = _get_f ();
				return _f;
			};
			std::function<std::vector<PointF_t> (int)> _get_pts = [&] (int _n) -> std::vector<PointF_t> {
				std::vector<PointF_t> _v;
				for (int i = 0; i < _n; ++i)
					_v.push_back (_get_pt ());
				return _v;
			};
			auto _scp = std::make_unique<SvgaClipPath_t> ();
			while (!_v.empty ()) {
				char _c = _get_ch ();
				bool _abs = _c >= 'A' && _c <= 'Z';
				switch (_c) {
				case 'M':
				case 'm':
					if (!_scp->m_tmp_closed)
						_scp->_StartPath ();
					_scp->_MoveTo (_get_pt (), _abs);
					break;
				case 'L':
				case 'l':
					_scp->_LineTo (_get_pt (), _abs);
					break;
				case 'H':
				case 'h':
					_scp->_HorizontalLineTo (_get_f (), _abs);
					break;
				case 'V':
				case 'v':
					_scp->_VerticalLineTo (_get_f (), _abs);
					break;
				case 'C':
				case 'c':
					_scp->_Bezier3 (_get_pts (3), _abs);
					break;
				case 'S':
				case 's':
					_scp->_SmoothCubicBezier2 (_get_pts (2), _abs);
					break;
				case 'Z':
				case 'z':
					_scp->_ClosePath ();
					break;
				case 'Q':
				case 'q':
					_scp->_QuadraticBezier2 (_get_pts (2), _abs);
					break;
				case 'T':
				case 't':
					_scp->_SmoothQuadraticBezier (_get_pt (), _abs);
					break;
				case 'A':
				case 'a':
					{
						float _rx, _ry, _x_axis_rotation;
						int _large_arc_flag, _sweep_flag;
						float _x2, _y2;
						_rx = _get_f ();
						_ry = _get_f ();
						_x_axis_rotation = _get_f ();
						_large_arc_flag = _get_i ();
						_sweep_flag = _get_i ();
						_x2 = _get_f ();
						_y2 = _get_f ();
						_scp->_ArcTo (_rx, _ry, _x_axis_rotation, _large_arc_flag, _sweep_flag, _x2, _y2, _abs);
					}
					break;
				}
			}
			return std::move (_scp);
			////
			//bool _init = true;
			//Gdiplus::PointF _p_start { 0, 0 };
			//Gdiplus::PointF _p_last { 0, 0 };
			////Gdiplus::PointF _ps [4] { {0, 0}, {0, 0}, {0, 0}, {0, 0} };
			//Gdiplus::PointF _p { 0, 0 };
			//while (!_v.empty ()) {
			//	switch (_get_ch ()) {
			//	case 'M':
			//		_p = _get_floats ();
			//		if (_init) {
			//			_init = false;
			//			_p_start = _p;
			//		}
			//		//m_path.moveTo (posX, posY);
			//		break;
			//	case 'm':
			//		_p.X = _p_last.X + _get_float ();
			//		_p.Y = _p_last.Y + _get_float ();
			//		if (_init) {
			//			_init = false;
			//			_p_start = _p;
			//		}
			//		//m_path.moveTo (posX, posY);
			//		break;
			//	case 'L':
			//		_p = _get_floats ();
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'l':
			//		_p.X = _p_last.X + _get_float ();
			//		_p.Y = _p_last.Y + _get_float ();
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'H':
			//		_p.X = _get_float ();
			//		_p.Y = _p_last.Y;
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'h':
			//		_p.X = _p_last.X + _get_float ();
			//		_p.Y = _p_last.Y;
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'V':
			//		_p.X = _p_last.X;
			//		_p.Y = _get_float ();
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'v':
			//		_p.X = _p_last.X;
			//		_p.Y = _p_last.Y + _get_float ();
			//		m_path.AddLine (_p_last, _p);
			//		//m_path.lineTo (posX, posY);
			//		break;
			//	case 'C':
			//	{
			//		Gdiplus::PointF _p1 { 0, 0 }, _p2 { 0, 0 };
			//		_p1 = _get_floats ();
			//		_p = _get_floats ();
			//		_p2 = _get_floats ();
			//		m_path.AddBezier (_p_last, _p1, _p, _p2);
			//		break;
			//	}
			//	case 'c':
			//	{
			//		Gdiplus::PointF _p1 { 0, 0 }, _p2 { 0, 0 };
			//		_p1 = _p_last + _get_floats ();
			//		_p = _p_last + _get_floats ();
			//		_p2 = _p_last + _get_floats ();
			//		m_path.AddBezier (_p_last, _p1, _p, _p2);
			//		break;
			//	}
			//	case 'S':
			//		if (_ps [1].X && _ps [1].Y && _ps [2].X && _ps [2].Y) {
			//			_ps [0] = _p_last;
			//			_ps [1].X = _p_last.X * 2 - _ps [2].X;
			//			_ps [1].Y = _p_last.Y * 2 - _ps [2].Y;
			//			_ps [2] = _get_floats ();
			//			_ps [3] = _get_floats ();
			//			_p = _ps [2];
			//			m_path.AddCurve (_ps, 4);
			//			//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
			//		} else {
			//			_ps [0] = _p_last;
			//			_ps [1] = _get_floats ();
			//			_ps [2] = _get_floats ();
			//			_p = _ps [2];
			//			m_path.AddCurve (_ps, 3);
			//			//m_path.quadTo (posX1, posY1, posX, posY);
			//		}
			//		break;
			//	case 's':
			//		if (_ps [1].X && _ps [1].Y && _ps [2].X && _ps [2].Y) {
			//			_ps [0] = _p_last;
			//			_ps [1].X = _p_last.X * 2 - _ps [2].X;
			//			_ps [1].Y = _p_last.Y * 2 - _ps [2].Y;
			//			_ps [2] = _get_floats ();
			//			_ps [3].X = _p_last.X + _get_float ();
			//			_ps [3].Y = _p_last.Y + _get_float ();
			//			_p = _ps [2];
			//			m_path.AddCurve (_ps, 4);
			//			//m_path.cubicTo (posX1, posY1, posX2, posY2, posX, posY);
			//		} else {
			//			_ps [0] = _p_last;
			//			_ps [1] = _get_floats ();
			//			_ps [2].X = _p_last.X + _get_float ();
			//			_ps [2].Y = _p_last.Y + _get_float ();
			//			_p = _ps [2];
			//			m_path.AddCurve (_ps, 3);
			//			//m_path.quadTo (posX1, posY1, posX, posY);
			//		}
			//		break;
			//	case 'Q':
			//		_ps [0] = _p_last;
			//		_ps [1] = _get_floats ();
			//		_ps [2] = _get_floats ();
			//		_p = _ps [2];
			//		m_path.AddCurve (_ps, 3);
			//		//m_path.quadTo (posX1, posY1, posX, posY);
			//		break;
			//	case 'q':
			//		_ps [0] = _p_last;
			//		_ps [1] = _get_floats ();
			//		_ps [2].X = _p_last.X + _get_float ();
			//		_ps [2].Y = _p_last.Y + _get_float ();
			//		_p = _ps [2];
			//		m_path.AddCurve (_ps, 3);
			//		//m_path.quadTo (posX1, posY1, posX, posY);
			//		break;
			//	case 'A':
			//		break;
			//	case 'a':
			//		break;
			//	case 'Z':
			//	case 'z':
			//		m_path.AddLine (_p_last, _p_start);
			//		//m_path.closeSubpath ();
			//		break;
			//	default:
			//		break;
			//	}
			//	_p_last = _p;
			//}
			//return true;
		}

		std::tuple<ResType_t, Image_t *> Clip (Image_t *_src_img) {
			Image_t *_img = ImageEngine_t::CreateImage ((int32_t) _src_img->GetWidth (), (int32_t) _src_img->GetHeight (), true);
			Gdiplus::Graphics _g (_img);
			_g.SetSmoothingMode (Gdiplus::SmoothingModeAntiAlias);
			//// test 0
			//Gdiplus::Pen _pen { Gdiplus::Color::Black };
			//_g.DrawPath (&_pen, &m_path);
			//// test 1
			//Gdiplus::SolidBrush _brush { Gdiplus::Color::Black };
			//_g.FillPath (&_brush, &m_path);
			// impl 0
			_g.SetClip (&m_path);
			Gdiplus::Rect _r { 0, 0, (INT) _src_img->GetWidth (), (INT) _src_img->GetHeight () };
			_g.DrawImage (_src_img, _r, 0, 0, _r.Width, _r.Height, Gdiplus::UnitPixel);
			_g.ResetClip ();
			return { ResType_t::NeedRelease, _img };
		}

	private:
		Gdiplus::GraphicsPath m_path;

		// tmps
		void _StartPath () { m_path.StartFigure (); m_tmp_closed = true; }
		void _ClosePath () { m_path.CloseFigure (); m_tmp_closed = true; m_tmp_last_c2 = m_tmp_last_pt = m_tmp_first_pt; }
		void _MoveTo (PointF_t _pt, bool _abs) {
			if (!_abs) { _pt = _pt + m_tmp_last_pt; }
			if (m_tmp_closed) { m_tmp_first_pt = _pt; m_tmp_closed = false; }
			m_tmp_last_c2 = m_tmp_last_pt = _pt;
		}
		void _LineTo (PointF_t _pt, bool _abs) {
			if (!_abs) { _pt = _pt + m_tmp_last_pt; }
			if (m_tmp_closed) { m_tmp_first_pt = _pt; m_tmp_closed = false; }
			m_path.AddLine (m_tmp_last_pt, _pt);
			m_tmp_last_c2 = m_tmp_last_pt = _pt;
		}
		void _HorizontalLineTo (float _f, bool _abs) { _LineTo (PointF_t { _f, m_tmp_last_pt.Y }, _abs); }
		void _VerticalLineTo (float _f, bool _abs) { _LineTo (PointF_t { m_tmp_last_pt.X, _f }, _abs); }
		void _Bezier3 (std::vector<PointF_t> _v, bool _abs) {
			if ((!m_tmp_closed) && (!_abs)) {
				_v [0] = _v [0] + m_tmp_last_pt;
				_v [1] = _v [1] + m_tmp_last_pt;
				_v [2] = _v [2] + m_tmp_last_pt;
			}
			if (m_tmp_closed) { m_tmp_first_pt = m_tmp_last_pt; m_tmp_closed = false; }
			m_path.AddBezier (m_tmp_last_pt, _v [0], _v [1], _v [2]);
			m_tmp_last_c2 = _v [1];
			m_tmp_last_pt = _v [2];
		}
		void _SmoothCubicBezier2 (std::vector<PointF_t> _v, bool _abs) {
			_v.insert (_v.begin (), PointF_t { m_tmp_last_pt.X * 2 - m_tmp_last_c2.X, m_tmp_last_pt.Y * 2 - m_tmp_last_c2.Y });
			_Bezier3 (_v, _abs);
		}
		void _QuadraticBezier2 (std::vector<PointF_t> _v, bool _abs) {
			_v.insert (_v.begin () + 1, _v [0]);
			_Bezier3 (_v, _abs);
		}
		void _SmoothQuadraticBezier (PointF_t _pt, bool _abs) {
			std::vector<PointF_t> _v;
			_v.push_back (m_tmp_last_pt);
			_v.push_back (PointF_t { m_tmp_last_pt.X * 2 - m_tmp_last_c2.X, m_tmp_last_pt.Y * 2 - m_tmp_last_c2.Y });
			_v.push_back (_v [1]);
			_v.push_back (_pt);
			_Bezier3 (_v, _abs);
		}
		void _ArcTo (float _rx, float _ry, float _x_axis_rotation, int _large_arc_flag, int _sweep_flag, float _x2, float _y2, bool _abs) {
			_rx = std::abs (_rx);
			_ry = std::abs (_ry);
			if (_rx < 1.0e-5f || _ry < 1.0e-5f) {
				_LineTo (PointF_t { _x2, _y2 }, _abs);
				return;
			}
			if ((!m_tmp_closed) && (!_abs)) { _x2 += m_tmp_last_pt.X; _y2 += m_tmp_last_pt.Y; }
			if (m_tmp_closed) { m_tmp_first_pt = m_tmp_last_pt; m_tmp_closed = false; }

			float _x1 = m_tmp_last_pt.X;
			float _y1 = m_tmp_last_pt.Y;

			float _cphi = cos (_x_axis_rotation);
			float _sphi = sin (_x_axis_rotation);

			//step1
			float _dx = _x1 - _x2;
			float _dy = _y1 - _y2;
			float _x1_ = (_cphi * _dx + _sphi * _dy) * 0.5f;
			float _y1_ = (-_sphi * _dx + _cphi * _dy) * 0.5f;

			//Ensure radii are large enough
			float _lambda = pow2 (_x1_ / _rx) + pow2 (_y1_ / _ry);
			if (_lambda > 1.0f) {
				_rx *= sqrt (_lambda);
				_ry *= sqrt (_lambda);
			}

			//step2
			float _sign = (_large_arc_flag == _sweep_flag) ? -1.0f : 1.0f;
			float _scale = sqrt ((pow2 (_rx * _ry) - pow2 (_rx * _y1_) - pow2 (_ry * _x1_)) / (pow2 (_rx * _y1_) + pow2 (_ry * _x1_)));
			float _cx_ = _sign * _scale * _rx * _y1_ / _ry;
			float _cy_ = -_sign * _scale * _ry * _x1_ / _rx;

			//step3
			float _cx = (_cphi * _cx_ - _sphi * _cy_) + (_x1 + _x2) * 0.5f;
			float _cy = (_sphi * _cx_ + _cphi * _cy_) + (_y1 + _y2) * 0.5f;

			//step4
			PointF_t _u ((_x1_ - _cx_) / _rx, (_y1_ - _cy_) / _ry);
			PointF_t _v ((-_x1_ - _cx_) / _rx, (-_y1_ - _cy_) / _ry);
			float _theta = _get_angle (PointF_t (1.0f, 0.0f), _u);
			float _deltatheta = _get_angle (_u, _v);

			if (_sweep_flag == 0 && _deltatheta > 0.0f)
				_deltatheta -= 360.0f;
			else if (_sweep_flag == 1 && _deltatheta < 0.0f)
				_deltatheta += 360.0f;

			//add a path
			m_path.AddArc (_cx - _rx, _cy - _ry, _rx * 2.0f, _ry * 2.0f, _theta, _deltatheta);

			m_tmp_last_c2 = m_tmp_last_pt = PointF_t { _x2, _y2 };
		}
		PointF_t m_tmp_last_pt { 0, 0 }, m_tmp_last_c2 { 0, 0 }, m_tmp_first_pt { 0, 0 };
		bool m_tmp_closed = true;
	};



	class SvgaVideoSpriteFrameImpl_t: public ISvgaVideoSpriteFrame_t {
	public:
		SvgaVideoSpriteFrameImpl_t () = default;
		virtual ~SvgaVideoSpriteFrameImpl_t () = default;
		std::tuple<ResType_t, Image_t *> Clip (Image_t *_src_img) override {
			return m_clip_path ? m_clip_path->Clip (_src_img) : std::make_tuple (ResType_t::Ignore, _src_img);
		}
		float GetAlpha () override { return m_alpha; }
		Transform_t GetTransform () override { return m_transform; }

		float m_alpha = 0.0f;
		RectF_t m_layout;
		Transform_t m_transform;
		std::unique_ptr<SvgaClipPath_t> m_clip_path;
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
		_ptr->m_clip_path = SvgaClipPath_t::Parse (_fe.clippath ());
		return _ptr;
	}
}
