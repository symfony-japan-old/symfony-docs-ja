.. index::
   single: Forms; Fields; reset

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/11/23

reset フィールドタイプ
================

.. versionadded:: 2.3
    ``reset`` タイプはSymfony 2.3から追加されました。

ボタンは全てのフィールドをデフォルトの値にリセットします。

+----------------------+---------------------------------------------------------------------+
| レンダリング           | ``input`` ``reset`` タグ                                             |
+----------------------+---------------------------------------------------------------------+
| 継承された            | - `attr`_                                                           |
| オプション              | - `disabled`_                                                       |
|                      | - `label`_                                                          |
|                      | - `translation_domain`_                                             |
+----------------------+---------------------------------------------------------------------+
| 親 タイプ             | :doc:`button</reference/forms/types/button>`                        |
+----------------------+---------------------------------------------------------------------+
| クラス                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ResetType` |
+----------------------+---------------------------------------------------------------------+

継承された オプション
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc

.. 2013/11/23 nntsugu 88b88e5bb47b0f39a634c6efc409902db3359a57