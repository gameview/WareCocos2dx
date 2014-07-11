//*************************************************************************
//	创建日期:	2013-8-30   14:14
//	文件名称:	CHtmlLabel.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	图文混排显示标签(目前只支持文字)
//				支持html标签解析，能够识别的标签
//				<a> 支持action 
//				<font> 支持size color
//				<img>
//				<br> 换行
//				支持裁剪区域显示
//*************************************************************************
#ifndef _CHTMLLABEL_H__
#define _CHTMLLABEL_H__

#include "cocos2d.h"
#include <string>
#include <map>
#include <list>
#include <vector>
#include "HtmlParse.h"

NS_CC_BEGIN

/// html label事件
struct IHtmlLabelEventHandler
{
	/**
	@brief 事件处理
	@param pszCommand 命令名称 <a href='XXX?slf=op&o=0p'> 如果XXX等于path,equip 视为查看装备和寻路
	@param pszParam 参数串
	@param bClose 是否需要关闭父窗口
	*/
	virtual void	onLinkEvent( const char* pszCommand, bool bSubmit, bool bClose ) = 0;

	/**
	@brief 元素回调 辑要上层创建的元素
	@param strElementName 元素名称 win input exp
	@param strAttr 属性串
	*/
	virtual void	onElement( const std::string& strElementName, CCSize size, std::map<std::string, std::string>& attrs, int nElementID ) = 0;

	/**
	@brief 数据标签回调
	@param
	*/
	virtual void	onDataElement( const std::string& strLabelName, const std::string& strValue ) = 0;

	/**
	@brief 开始画外部元素
	@param
	*/
	virtual void	onBeginDrawElement() = 0;

	/**
	@brief 画元素回调
	@param
	*/
	virtual void	onDrawElement( int nElementID, CCPoint& pos ) = 0;
};

class CCHtmlLabel : public CCNode
{
public:
	CCHtmlLabel(void);
	~CCHtmlLabel(void);

	/** creates a CCHtmlLabel with a font name and font size in points 目前没有根据宽度自动分行(需要支持)
	@since v2.0.1
	@param size 区域大小
	@param nMargin 边距(像素)
	@param nLineSpace 行间距
	*/
	static CCHtmlLabel * create( int nMargin = PAN_WIDTH, int nLineSpace = LINE_SPACE );

	/**
	*  Initializes the instance of CCNode
	*  @return Whether the initialization was successful.
	*/
	bool			init( int nMargin, int nLineSpace );

	void			setSize(const CCSize& size);

	CCSize			getSize() { return m_Size; }

	/**
	@brief 设置默认字体名
	@param
	*/
	void			setFontName( const char* pFontName );

	/**
	@brief 设置字体大小
	@param
	*/
	void			setFontSize( int nSize );

	/**
	@brief 
	@param 对象更新
	*/
	virtual void	update(float delta);

	/**
	@brief 启动回滚
	@param
	*/
	virtual void	startRollback();

	/**
	@brief 事件处理
	@param
	*/
	void			setEventHandler( IHtmlLabelEventHandler* pHandler ) { m_pHandler = pHandler; }

	/**
	@brief 添加字符串，支持html格式 默认往后追加
	@param
	*/
	void			addString( const std::string& strContent );

	/**
	@brief 清除数据
	@param
	*/
	void			clearString();

	/**
	@brief 修改宽度
	@param
	*/
	void			changeWidth( float fWidth );

	/**
	@brief 获取内容的像素大小
	@param
	*/
	CCSize			getPixelSize();

	/**
	@brief 设置是否强制置顶
	@param
	*/
	void			setForceUpdateOffset( bool bForceUpdateOffset ) { m_bForceUpdateOffset = bForceUpdateOffset; m_bNeedUpdate = true; }

	/**
	@brief 设置偏移
	@param
	*/
	void			setOffset( const CCPoint& offset );

	/**
	@brief 获取偏移
	@param
	*/
	CCPoint			getOffset() { return m_ptOffset; }

	/**
	@brief 滚动到顶部
	@param
	*/
	void			scrollToTop();

	/**
	@brief 滚动到底部
	@param
	*/
	void			scrollToBottom();

	/**
	@brief 设置最大内容行数
	@param
	*/
	void			setMaxLine( int nLineNum ) { m_nMaxLine = nLineNum; }

	/**
	@brief 事件处理
	@param ptLoc 触摸位置
	*/
	void			onEventProcess( const CCPoint& ptLoc );

private:

	/**
	@brief 解析命令串
	@retrun 是否需要关闭窗口
	*/
	void			parseCommand( const std::string& strCommand, std::string& strAction, std::string& strParam );

	/**
	@brief 画一行
	@param
	*/
	void			drawLine( int nXStartPos, int nYStartpos, CHtmlParse::LineContainerEx& line );

	/**
	@brief 回滚函数调用
	@param
	*/
	void			rollback( float dt );

	/**
	@brief 获取内容总行数
	@param
	*/
	int				getLine();

	/**
	@brief 删除队列前端多少行
	@param
	*/
	void			removeLines( int nLines );


private:

	struct SContextInfo
	{
		std::string	strContent;		/// 内容字串支持html格式
		CHtmlParse::Lines vLines;	/// 行数据
		CHtmlParse::ExLines vExLines;	/// 行数据

		SContextInfo() : strContent("")
		{
			vLines.clear();
			vExLines.clear();
		}
	};

	CCSprite*					m_pSprite;			/// 图片精灵对象
	CCRenderTexture*			m_pRenderTarget;	/// rednertarget

	std::vector<SContextInfo>	m_vecContent;		/// 内容数据

	std::vector<CHtmlParse::LineContainer*>	    m_vecShowLines;		/// 内容数据

	std::vector<CHtmlParse::LineContainerEx*>	m_vecExShowLines;

	CCPoint						m_ptOffset;			/// 偏移(像素单位)

	/// 回滚参数
	float						m_fRollbackSpeed;		/// 回滚速度
	float						m_fRollbackRemainTime;	/// 回滚剩余时间
	float						m_fDir;					/// 回滚移动方向  1 up -1 down
	float						m_fTarget;				/// 目标位置

	CCSize						m_Size;				/// 大小
	int							m_nMargin;			/// 边距
	int							m_nLineSpace;		/// 行间距

	int							m_nMaxLine;			/// 最大内容行

	bool						m_bForceUpdateOffset;/// 添加内容时（或者更新时）是否强制更新偏移
	bool						m_bNeedUpdate;		/// 是否需要更新
	
	IHtmlLabelEventHandler*		m_pHandler;			/// 事件回调
	CHtmlParse					m_HtmlParse;		/// html解析器
};

NS_CC_END

#endif // _CHTMLLABEL_H__

