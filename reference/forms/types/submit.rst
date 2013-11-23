.. index::
   single: Forms; Fields; submit

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/11/23

submit Field Type
=================

.. versionadded:: 2.3
    ``submit`` タイプはSymfony 2.3で追加されました

submitボタン

+----------------------+----------------------------------------------------------------------+
| 対応するタグ           | ``input`` ``submit`` tag                                             |
+----------------------+----------------------------------------------------------------------+
| 継承されたオプション    | - `attr`_                                                            |
|                      | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `translation_domain`_                                              |
+----------------------+----------------------------------------------------------------------+
| 親タイプ              | :doc:`button</reference/forms/types/button>`                         |
+----------------------+----------------------------------------------------------------------+
| クラス                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SubmitType` |
+----------------------+----------------------------------------------------------------------+

Submitボタンには
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked` という付加的なメソッドがあります。
このメソッドでこのボタンがフォームのサブミットに利用されたかどうかをチェックできます。
特に :ref:`複数のサブミットボタンを持つフォームの場合 <book-form-submitting-multiple-buttons>` に便利です。

    if ($form->get('save')->isClicked()) {
        // ...
    }

継承されたオプション
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc

.. 2013/11/23 kseta bdb3715450bfc9e97a0a8ac3def4f3911e730283