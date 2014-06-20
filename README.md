Drupal Special Topic Module | Drupal 专题页面模块
=============

Drupal Special Topic模块提供一种通用并且可定制的Drupal专题页面功能。

这里的专题页面，指的是将网站中与某个话题相关的内容聚合到一个页面中，具有特殊的布局与样式设计。国内常见的专题页面大多以区块的形式展现，不同的位置显示不同的内容列表，有链接与图文混合等展示形式。

本模块提供Special Topic内容类型，其功能类似于 Panel Node + EntityReference 的相加，用户可以利用Panel的功能来定制页面结构以及各部分显示的内容，又可以在其他Node类型中添加 special_topic field，指定其所属的专题以及在专题中的位置。

模块提供CTools的content_types插件，通过此方式来将专题相关的内容通过比较简单的方式提取出来，输出标准的content_types。

同时，模块提供了topic_render插件类型，用户可以通过实现此插件为Special Topic相关的content_types提供theme功能，用不同的形式展现内容。

模块依赖于ctools与panel。
